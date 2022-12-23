#include "server_worker.h"
#include "benchmark_control_rpc.h"
#include "communicator.h"
#include "frame.h"
#include "rcc_service.h"
#include "scheduler.h"
#include "sharding.h"
#include "cdc_client.h"

namespace rococo {


void ServerWorker::SetupHeartbeat() {
    bool hb = Config::GetConfig()->do_heart_beat();
    if (!hb) return;
    auto timeout = Config::GetConfig()->get_ctrl_timeout();
    scsi_ = new ServerControlServiceImpl(timeout);
    int n_io_threads = 1;
    svr_hb_poll_mgr_g = new rrr::PollMgr(n_io_threads);
    hb_thread_pool_g = new rrr::ThreadPool(1);
    hb_rpc_server_ = new rrr::Server(svr_hb_poll_mgr_g, hb_thread_pool_g);
    hb_rpc_server_->reg(scsi_);

    auto port = this->site_info_->port + ServerWorker::CtrlPortDelta;
    std::string addr_port = std::string("0.0.0.0:") +
                            std::to_string(port);
    hb_rpc_server_->start(addr_port.c_str());
    if (hb_rpc_server_ != nullptr) {
        // Log_info("notify ready to control script for %s", bind_addr.c_str());
        scsi_->set_ready();
    }
    Log_info("heartbeat setup for %s on %s",
             this->site_info_->name.c_str(), addr_port.c_str());
}

void ServerWorker::SetupBase() {
    auto config = Config::GetConfig();
    dtxn_frame_ = Frame::GetFrame(config->cc_mode_);
    dtxn_frame_->site_info_ = site_info_;
    dtxn_frame_->cdc_client_->my_par_id_ = site_info_->partition_id_;

    // this needs to be done before poping table
    sharding_ = dtxn_frame_->CreateSharding(Config::GetConfig()->sharding_);
    sharding_->BuildTableInfoPtr();

    verify(txn_reg_ == nullptr);
    txn_reg_ = new TxnRegistry();
    dtxn_sched_ = dtxn_frame_->CreateScheduler();
    dtxn_sched_->txn_reg_ = txn_reg_;
    dtxn_sched_->SetPartitionId(site_info_->partition_id_);
    dtxn_sched_->loc_id_ = site_info_->locale_id;
    dtxn_sched_->site_id_ = site_info_->id;
    dtxn_sched_->my_rid_ = dtxn_sched_->site_id_ / (Config::GetConfig()->n_shard_per_region_ * N_REP_PER_SHARD);
    sharding_->dtxn_sched_ = dtxn_sched_;

    if (config->IsReplicated() &&
        config->ab_mode_ != config->cc_mode_) {
        rep_frame_ = Frame::GetFrame(config->ab_mode_);
        rep_frame_->site_info_ = site_info_;
        rep_sched_ = rep_frame_->CreateScheduler();
        rep_sched_->txn_reg_ = txn_reg_;
        rep_sched_->loc_id_ = site_info_->locale_id;
        dtxn_sched_->rep_frame_ = rep_frame_;
        dtxn_sched_->rep_sched_ = rep_sched_;
    }
    // add callbacks to execute commands to rep_sched_
    if (rep_sched_ && dtxn_sched_) {
        rep_sched_->RegLearnerAction(std::bind(&Scheduler::OnLearn,
                                               dtxn_sched_,
                                               std::placeholders::_1));
    }
}

void ServerWorker::PopTable() {
    // populate table
    int ret = 0;
    // get all tables
    std::vector<std::string> table_names;

    Log_info("start data population for site %d", site_info_->id);
    ret = sharding_->GetTableNames(site_info_->partition_id_, table_names);
    verify(ret > 0);

    for (auto table_name : table_names) {
        mdb::Schema *schema = new mdb::Schema();
        mdb::symbol_t symbol;
        sharding_->init_schema(table_name, schema, &symbol);
        mdb::Table *tb;

        switch (symbol) {
            case mdb::TBL_SORTED:
                tb = new mdb::SortedTable(table_name, schema);
                break;
            case mdb::TBL_UNSORTED:
                tb = new mdb::UnsortedTable(table_name, schema);
                break;
            case mdb::TBL_SNAPSHOT:
                tb = new mdb::SnapshotTable(table_name, schema);
                break;
            default:
                verify(0);
        }
        dtxn_sched_->reg_table(table_name, tb);
    }
    verify(sharding_);
    sharding_->PopulateTables(site_info_->partition_id_);
    Log_debug("data populated for site: %x, partition: %x",
              site_info_->id, site_info_->partition_id_);
    verify(ret > 0);
}

void ServerWorker::RegPiece() {
    auto benchmark = Config::GetConfig()->get_benchmark();
    Piece *piece = Piece::get_piece(benchmark);
    verify(txn_reg_ != nullptr);
    verify(sharding_ != nullptr);
    piece->sss_ = sharding_;
    piece->txn_reg_ = txn_reg_;
    piece->reg_all();
}

void ServerWorker::SetupGlobalRaftService() {
    Log_info("%s called", __FUNCTION__);

    auto global_cfg = Config::GetConfig();

    std::string bind_addr = std::string("0.0.0.0:").append(std::to_string(global_cfg->raft_port_map_[site_info_->proc_name]));

    int n_io_threads = 1;
    svr_raft_poll_mgr_ = new rrr::PollMgr(n_io_threads);

    //create service
    raft_service_ = new AuxiliaryRaftImpl(site_info_->proc_name);

    raft_thread_pool_ = new rrr::ThreadPool(1);

    raft_rpc_server_ = new rrr::Server(svr_raft_poll_mgr_, raft_thread_pool_);

    raft_rpc_server_->reg(raft_service_);

    raft_rpc_server_->start(bind_addr.c_str());

    Log_info("Auxiliary Raft server started");
}


void ServerWorker::SetupDastManagerService(Frame *f) {
    Log_info("%s called", __FUNCTION__);

    auto global_cfg = Config::GetConfig();

    verify(f != nullptr);

    uint32_t region_id = f->site_info_->partition_id_ / Config::GetConfig()->n_shard_per_region_;

    //default 9000, 9001, 9002,
    //TODO: move this to config
    std::string bind_addr = std::string("0.0.0.0:").append(std::to_string(9000 + region_id));

    int n_io_threads = 1;
    dast_manager_poll_mgr_ = new rrr::PollMgr(n_io_threads);

    dast_manager_service_ = new DastManager(f);
    dast_manager_thread_pool_ = new rrr::ThreadPool(1);
    dast_manager_rpc_server_ = new rrr::Server(dast_manager_poll_mgr_, dast_manager_thread_pool_);
    dast_manager_rpc_server_->reg(dast_manager_service_);

    dast_manager_rpc_server_->start(bind_addr.c_str());

    Log_info("Dast manager started on port %s", bind_addr.c_str());
}

void ServerWorker::SetupSlogRegionManagerService(Frame *f) {
    Log_info("%s called", __FUNCTION__);

    auto global_cfg = Config::GetConfig();

    verify(f != nullptr);
    siteid_t site_id = f->site_info_->id;

    //default 9000, 9001, 9002,
    //TODO: move this to config
    std::string bind_addr = std::string("0.0.0.0:").append(std::to_string(9000 + site_id));

    int n_io_threads = 1;
    slog_manager_poll_mgr_ = new rrr::PollMgr(n_io_threads);

    slog_manager_service_ = new SlogRegionManager(f);
    slog_manager_service_->partition_id_ = f->site_info_->partition_id_;

    slog_manager_thread_pool_ = new rrr::ThreadPool(1);
    slog_manager_rpc_server_ = new rrr::Server(slog_manager_poll_mgr_, slog_manager_thread_pool_);
    slog_manager_rpc_server_->reg(slog_manager_service_);

    slog_manager_rpc_server_->start(bind_addr.c_str());

    Log_info("Slog region manager started on port %s", bind_addr.c_str());
}

void ServerWorker::SetupService() {

    auto global_cfg = Config::GetConfig();
    verify(global_cfg != nullptr);
    verify(site_info_ != nullptr);

    if (global_cfg->get_mode() == MODE_SLOG) {

        if (global_cfg->raft_port_map_.count(site_info_->proc_name) != 0) {
            Log_info("proc name %s, going to setup raft service", site_info_->proc_name.c_str());
            SetupGlobalRaftService();
        } else {
            Log_info("proc name %s, no raft service", site_info_->proc_name.c_str());
        }

        if (site_info_->id % (N_REP_PER_SHARD * Config::GetConfig()->n_shard_per_region_) == 0 || site_info_->id % (N_REP_PER_SHARD * Config::GetConfig()->n_shard_per_region_) == 1) {
            //Let the manager co-locate with the first and second site in each region.
            Log_info("[Slog Manager] going to setup slog manager service, my site = %hu", site_info_->id);
            SetupSlogRegionManagerService(dtxn_frame_);
        } else {
            Log_info("[Slog Manager] not setting slog manager service, my site = %hu", site_info_->id);
        }

    }

    if (global_cfg->get_mode() == MODE_CHRONOS) {
        int site_per_par = global_cfg->GetPartitionSize(site_info_->partition_id_);

        if (site_info_->id % (site_per_par * Config::GetConfig()->n_shard_per_region_) == 0) {
            //Let the manager co-locate with the first site in each region.
            Log_info("[Dast Manager] going to setup Dast manager service, my site = %hu", site_info_->id);
            SetupDastManagerService(dtxn_frame_);
        } else {
            Log_info("[Dast Manager] not setting Dast manager service, my site = %hu", site_info_->id);
        }
    }

    Log_info("enter %s for %s @ %s", __FUNCTION__,
             this->site_info_->name.c_str(),
             site_info_->GetBindAddress().c_str());

    int ret;
    // set running mode and initialize transaction manager.
    std::string bind_addr = site_info_->GetBindAddress();

    // init rrr::PollMgr 1 threads
    int n_io_threads = 1;
    svr_poll_mgr_ = new rrr::PollMgr(n_io_threads);

    // init service implementation

    if (dtxn_frame_ != nullptr) {
        services_ = dtxn_frame_->CreateRpcServices(site_info_->id,
                                                   dtxn_sched_,
                                                   svr_poll_mgr_,
                                                   scsi_);
    }

    if (rep_frame_ != nullptr) {
        auto s2 = rep_frame_->CreateRpcServices(site_info_->id,
                                                rep_sched_,
                                                svr_poll_mgr_,
                                                scsi_);

        services_.insert(services_.end(), s2.begin(), s2.end());
    }

    auto &alarm = TimeoutALock::get_alarm_s();
    ServerWorker::svr_poll_mgr_->add(&alarm);

    uint32_t num_threads = 1;
    thread_pool_g = new base::ThreadPool(num_threads);

    // init rrr::Server
    rpc_server_ = new rrr::Server(svr_poll_mgr_, thread_pool_g);

    // reg services
    for (auto service : services_) {
        rpc_server_->reg(service);
    }

    // start rpc server
    Log_debug("starting server at %s", bind_addr.c_str());
    ret = rpc_server_->start(bind_addr.c_str());
    if (ret != 0) {
        Log_fatal("server launch failed.");
    }

    Log_info("Server %s ready at %s",
             site_info_->name.c_str(),
             bind_addr.c_str());
}

void ServerWorker::WaitForShutdown() {
    Log_debug("%s", __FUNCTION__);
    if (hb_rpc_server_ != nullptr) {
        scsi_->wait_for_shutdown();
        delete hb_rpc_server_;
        delete scsi_;
        svr_hb_poll_mgr_g->release();
        hb_thread_pool_g->release();

        for (auto service : services_) {
            if (DepTranServiceImpl *s = dynamic_cast<DepTranServiceImpl *>(service)) {
                auto &recorder = s->recorder_;
                if (recorder) {
                    auto n_flush_avg_ = recorder->stat_cnt_.peek().avg_;
                    auto sz_flush_avg_ = recorder->stat_sz_.peek().avg_;
                    Log::info("Log to disk, average log per flush: %lld,"
                              " average size per flush: %lld",
                              n_flush_avg_, sz_flush_avg_);
                }
            }
        }
    }

    Log_debug("exit %s", __FUNCTION__);
}

void ServerWorker::SetupCommo() {
    verify(svr_poll_mgr_ != nullptr);
    if (dtxn_frame_) {
        dtxn_commo_ = dtxn_frame_->CreateCommo();
        if (dtxn_commo_) {
            dtxn_commo_->loc_id_ = site_info_->locale_id;
        }
        dtxn_sched_->commo_ = dtxn_commo_;

        if (dast_manager_service_){
            dast_manager_service_->commo_ = dtxn_commo_;
        }

        if (slog_manager_service_){
            slog_manager_service_->commo_ = dtxn_commo_;
        }
    }
    if (rep_frame_) {
        rep_commo_ = rep_frame_->CreateCommo();
        if (rep_commo_) {
            rep_commo_->loc_id_ = site_info_->locale_id;
        }
        rep_sched_->commo_ = rep_commo_;
    }

    if (raft_rpc_server_) {
        raft_service_->SetupCommo();
    }
}

void ServerWorker::ShutDown() {
    Log_debug("deleting services, num: %d", services_.size());
    delete rpc_server_;
    for (auto service : services_) {
        delete service;
    }
    thread_pool_g->release();
    svr_poll_mgr_->release();

    delete raft_rpc_server_;
    delete raft_service_;

    raft_thread_pool_->release();
    svr_raft_poll_mgr_->release();
    Log_info("%s returned", __PRETTY_FUNCTION__);
}
}// namespace rococo