#include <bench/rw_benchmark/generator.h>
#include <bench/retwis/generator.h>
#include "__dep__.h"
#include "frame.h"
#include "config.h"
#include "rcc/rcc_row.h"
#include "ro6/ro6_row.h"
#include "marshal-value.h"
#include "coordinator.h"
#include "dtxn.h"
#include "rcc_service.h"
#include "scheduler.h"
#include "none/coord.h"
#include "none/sched.h"
#include "rcc/coord.h"
#include "ro6/ro6_coord.h"
#include "tpl/coord.h"
#include "tpl/exec.h"
#include "occ/dtxn.h"
#include "occ/coord.h"
#include "occ/exec.h"

#include "bench/tpcc_real_dist/sharding.h"
#include "bench/tpcc/generator.h"



// for tpca benchmark
#include "bench/tpca/piece.h"
#include "bench/tpca/chopper.h"
#include "bench/tpca/sharding.h"
#include "bench/tpca/generator.h"

// tpcc benchmark
#include "bench/tpcc/piece.h"
#include "bench/tpcc/chopper.h"
#include "bench/tpcc/sharding.h"

// tpcc dist partition benchmark
#include "bench/tpcc_dist/piece.h"
#include "bench/tpcc_dist/chopper.h"

// tpcc real dist partition benchmark
#include "bench/tpcc_real_dist/piece.h"
#include "bench/tpcc_real_dist/chopper.h"
#include "bench/tpcc_real_dist/generator.h"

// rw benchmark
#include "bench/rw_benchmark/piece.h"
#include "bench/rw_benchmark/chopper.h"
#include "bench/rw_benchmark/sharding.h"

// micro bench
#include "bench/micro/piece.h"
#include "bench/micro/chopper.h"

//retwis bench 
#include "bench/retwis/piece.h"
#include "bench/retwis/chopper.h"
#include "bench/retwis/sharding.h"

// ch benchmark
#include "bench/chBenchmark/piece.h"
#include "bench/chBenchmark/chopper.h"
#include "bench/chBenchmark/sharding.h"
#include "bench/chBenchmark/generator.h"


#include "tpl/sched.h"
#include "occ/sched.h"

#include "deptran/mdcc/coordinator.h"
#include "deptran/mdcc/executor.h"
#include "deptran/mdcc/services.h"
#include "deptran/mdcc/MdccDTxn.h"

#include "cdc_client.h"

namespace rococo {

    Frame::Frame(int mode) : mode_(mode) {
        this->cdc_client_ = new CdcClient();
    }

    Frame *Frame::RegFrame(int mode,
                           function<Frame *()> frame_init) {
        auto &mode_to_frame = Frame::ModeToFrame();
        auto it = mode_to_frame.find(mode);
        verify(it == mode_to_frame.end());
        mode_to_frame[mode] = frame_init;
        return frame_init();
    }

    Frame *Frame::GetFrame(int mode) {
        Frame *frame = nullptr;
        // some built-in mode
        switch (mode) {
            case MODE_NONE:
            case MODE_MDCC:
            case MODE_2PL:
            case MODE_OCC:
                frame = new Frame(mode);
                break;
            default:
                auto &mode_to_frame = Frame::ModeToFrame();
                auto it = mode_to_frame.find(mode);
                verify(it != mode_to_frame.end());
                frame = it->second();
        }

        return frame;
    }

    int Frame::Name2Mode(string name) {
        auto &m = Frame::FrameNameToMode();
        return m.at(name);
    }

    Frame *Frame::GetFrame(string name) {
        return GetFrame(Name2Mode(name));
    }

    Frame *Frame::RegFrame(int mode,
                           vector <string> names,
                           function<Frame *()> frame) {
        for (auto name: names) {
            //verify(frame_name_mode_s.find(name) == frame_name_mode_s.end());
            auto &m = Frame::FrameNameToMode();
            m[name] = mode;
        }
        return RegFrame(mode, frame);
    }

    Sharding *Frame::CreateSharding() {
        Sharding *ret;
        auto bench = Config::config_s->benchmark_;
        switch (bench) {
            case TPCC_REAL_DIST_PART:
                ret = new TpccdSharding();
                break;
            case TPCC:
                ret = new TpccSharding();
                break;
            case RW_BENCHMARK:
                ret = new RWBenchmarkSharding();
                break;
            case TPCA:
                ret = new TpcaSharding();
                break;
            case RETWIS:
                ret = new RetwisBenchmarkSharding();
                break;
            case CH_BENCHMARK:
                ret = new ChBenchmarkSharding();
                break;
            default:
                verify(0);
        }
        return ret;
    }

    Sharding *Frame::CreateSharding(Sharding *sd) {
        verify(sd != nullptr);
        Sharding *ret = CreateSharding();
        *ret = *sd;
        ret->frame_ = this;
        return ret;
    }

    mdb::Row *Frame::CreateRow(const mdb::Schema *schema,
                               vector <Value> &row_data) {
//  auto mode = Config::GetConfig()->cc_mode_;
        auto mode = mode_;
        mdb::Row *r = nullptr;
        switch (mode) {
            case MODE_2PL:
                r = mdb::FineLockedRow::create(schema, row_data);
                break;
            case MODE_NONE: // FIXME
            case MODE_MDCC:
            case MODE_OCC:
                r = mdb::VersionedRow::create(schema, row_data);
                break;
            case MODE_RO6:
                r = RO6Row::create(schema, row_data);
                break;
            default:
                verify(0);
        }
        return r;
    }

    Coordinator *Frame::CreateCoord(cooid_t coo_id,
                                    Config *config,
                                    int benchmark,
                                    ClientControlServiceImpl *ccsi,
                                    uint32_t id,
                                    TxnRegistry *txn_reg) {
        // TODO: clean this up; make Coordinator subclasses assign txn_reg_
        Coordinator *coo;
        auto attr = this;
//  auto mode = Config::GetConfig()->cc_mode_;
        auto mode = mode_;
        switch (mode) {
            case MODE_2PL:
                coo = new TPLCoord(coo_id,
                                   benchmark,
                                   ccsi,
                                   id);
                ((Coordinator *) coo)->txn_reg_ = txn_reg;
                break;
            case MODE_OCC:
            case MODE_RPC_NULL:
                coo = new OCCCoord(coo_id,
                                   benchmark,
                                   ccsi,
                                   id);
                ((Coordinator *) coo)->txn_reg_ = txn_reg;
                break;
            case MODE_RCC:
                coo = new RccCoord(coo_id,
                                   benchmark,
                                   ccsi,
                                   id);
                ((Coordinator *) coo)->txn_reg_ = txn_reg;
                break;
            case MODE_RO6:
                coo = new RO6Coord(coo_id,
                                   benchmark,
                                   ccsi,
                                   id);
                ((Coordinator *) coo)->txn_reg_ = txn_reg;
                break;
            case MODE_NONE:
                coo = new NoneCoord(coo_id,
                                    benchmark,
                                    ccsi,
                                    id);
                ((Coordinator *) coo)->txn_reg_ = txn_reg;
                break;
            case MODE_MDCC:
                coo = (Coordinator *) new mdcc::MdccCoordinator(coo_id, id, config, ccsi);
                break;
            default:
                verify(0);
        }
        coo->frame_ = this;
        return coo;
    }

    void Frame::GetTxnTypes(std::map <int32_t, std::string> &txn_types) {
        auto benchmark_ = Config::config_s->benchmark_;
        switch (benchmark_) {
            case TPCA:
                txn_types[TPCA_PAYMENT] = std::string(TPCA_PAYMENT_NAME);
                break;
            case TPCC:
            case TPCC_DIST_PART:
            case TPCC_REAL_DIST_PART:
                txn_types[TPCC_NEW_ORDER] = std::string(TPCC_NEW_ORDER_NAME);
                txn_types[TPCC_PAYMENT] = std::string(TPCC_PAYMENT_NAME);
                txn_types[TPCC_STOCK_LEVEL] = std::string(TPCC_STOCK_LEVEL_NAME);
                txn_types[TPCC_DELIVERY] = std::string(TPCC_DELIVERY_NAME);
                txn_types[TPCC_ORDER_STATUS] = std::string(TPCC_ORDER_STATUS_NAME);
                break;
            case CH_BENCHMARK:
                txn_types[CHBENCHMARK_NEW_ORDER] = std::string(CHBENCHMARK_NEW_ORDER_NAME);
                txn_types[CHBENCHMARK_PAYMENT] = std::string(CHBENCHMARK_PAYMENT_NAME);
                txn_types[CHBENCHMARK_STOCK_LEVEL] = std::string(CHBENCHMARK_STOCK_LEVEL_NAME);
                txn_types[CHBENCHMARK_DELIVERY] = std::string(CHBENCHMARK_DELIVERY_NAME);
                txn_types[CHBENCHMARK_ORDER_STATUS] = std::string(CHBENCHMARK_ORDER_STATUS_NAME);
                txn_types[CHBENCHMARK_QUERY1] = std::string(CHBENCHMARK_QUERY1_NAME);
                txn_types[CHBENCHMARK_QUERY2] = std::string(CHBENCHMARK_QUERY2_NAME);
                txn_types[CHBENCHMARK_QUERY3] = std::string(CHBENCHMARK_QUERY3_NAME);
                txn_types[CHBENCHMARK_QUERY4] = std::string(CHBENCHMARK_QUERY4_NAME);
                txn_types[CHBENCHMARK_QUERY5] = std::string(CHBENCHMARK_QUERY5_NAME);
                txn_types[CHBENCHMARK_QUERY6] = std::string(CHBENCHMARK_QUERY6_NAME);
                txn_types[CHBENCHMARK_QUERY7] = std::string(CHBENCHMARK_QUERY7_NAME);
                txn_types[CHBENCHMARK_QUERY8] = std::string(CHBENCHMARK_QUERY8_NAME);
                txn_types[CHBENCHMARK_QUERY9] = std::string(CHBENCHMARK_QUERY9_NAME);
                txn_types[CHBENCHMARK_QUERY10] = std::string(CHBENCHMARK_QUERY10_NAME);
                txn_types[CHBENCHMARK_QUERY11] = std::string(CHBENCHMARK_QUERY11_NAME);
                txn_types[CHBENCHMARK_QUERY12] = std::string(CHBENCHMARK_QUERY12_NAME);
                txn_types[CHBENCHMARK_QUERY13] = std::string(CHBENCHMARK_QUERY13_NAME);
                txn_types[CHBENCHMARK_QUERY14] = std::string(CHBENCHMARK_QUERY14_NAME);
                txn_types[CHBENCHMARK_QUERY15] = std::string(CHBENCHMARK_QUERY15_NAME);
                txn_types[CHBENCHMARK_QUERY16] = std::string(CHBENCHMARK_QUERY16_NAME);
                txn_types[CHBENCHMARK_QUERY17] = std::string(CHBENCHMARK_QUERY17_NAME);
                txn_types[CHBENCHMARK_QUERY18] = std::string(CHBENCHMARK_QUERY18_NAME);
                txn_types[CHBENCHMARK_QUERY19] = std::string(CHBENCHMARK_QUERY19_NAME);
                txn_types[CHBENCHMARK_QUERY20] = std::string(CHBENCHMARK_QUERY20_NAME);
                txn_types[CHBENCHMARK_QUERY21] = std::string(CHBENCHMARK_QUERY21_NAME);
                txn_types[CHBENCHMARK_QUERY22] = std::string(CHBENCHMARK_QUERY22_NAME);
                break;
            case RW_BENCHMARK:
                txn_types[RW_BENCHMARK_W_TXN] = std::string(RW_BENCHMARK_W_TXN_NAME);
                txn_types[RW_BENCHMARK_R_TXN] = std::string(RW_BENCHMARK_R_TXN_NAME);
                break;
            case MICRO_BENCH:
                txn_types[MICRO_BENCH_R] = std::string(MICRO_BENCH_R_NAME);
                txn_types[MICRO_BENCH_W] = std::string(MICRO_BENCH_W_NAME);
                break;
            case RETWIS:
                txn_types[RETWIS_ADD_USERS] = std::string(RETWIS_ADD_USERS_NAME);
                txn_types[RETWIS_FOLLOW] = std::string(RETWIS_FOLLOW_NAME);
                txn_types[RETWIS_POST_TWEET] = std::string(RETWIS_POST_TWEET_NAME);
                txn_types[RETWIS_GET_TIMELINE] = std::string(RETWIS_GET_TIMELINE_NAME);
                break;
            default:
                Log_fatal("benchmark not implemented");
                verify(0);
        }
    }

    TxnCommand *Frame::CreateTxnCommand(TxnRequest &req, TxnRegistry *reg) {
        auto benchmark = Config::config_s->benchmark_;
        TxnCommand *cmd = NULL;
        switch (benchmark) {
            case TPCA:
                verify(req.txn_type_ == TPCA_PAYMENT);
                cmd = new TpcaPaymentChopper();
                break;
            case CH_BENCHMARK:
                cmd = new ChbenchmarkTxn();
                break;
            case TPCC:
                cmd = new TpccTxn();
                break;
            case TPCC_DIST_PART:
                cmd = new TpccDistChopper();
                break;
            case TPCC_REAL_DIST_PART:
                cmd = new TpccRealDistChopper();
                break;
            case RW_BENCHMARK:
                cmd = new RWChopper();
                break;
            case MICRO_BENCH:
                cmd = new MicroTxnCmd();
                break;
            case RETWIS:
                cmd = new RetwisTxn();
                break;
            default:
                verify(0);
        }
        verify(cmd != NULL);
        cmd->txn_reg_ = reg;
        cmd->sss_ = Config::GetConfig()->sharding_;
        cmd->Init(req);
        verify(cmd->n_pieces_dispatchable_ > 0);
        return cmd;
    }

    TxnCommand *Frame::CreateChopper(TxnRequest &req, TxnRegistry *reg) {
        return CreateTxnCommand(req, reg);
    }

    Communicator *Frame::CreateCommo(PollMgr *pollmgr) {
        commo_ = new RococoCommunicator;
        return commo_;
    }

    DTxn *Frame::CreateDTxn(epoch_t epoch, txnid_t tid,
                            bool ro, Scheduler *mgr) {
        DTxn *dtxn = nullptr;

        switch (mode_) {
            case MODE_2PL:
                dtxn = new TPLDTxn(epoch, tid, mgr);
                break;
            case MODE_MDCC:
                dtxn = new mdcc::MdccDTxn(tid, mgr);
                break;
            case MODE_OCC:
                dtxn = new OccDTxn(epoch, tid, mgr);
                break;
            case MODE_NONE:
                dtxn = new TPLDTxn(epoch, tid, mgr);
                break;
            case MODE_RCC:
                dtxn = new RccDTxn(epoch, tid, mgr, ro);
                break;
            case MODE_RO6:
                dtxn = new RO6DTxn(tid, mgr, ro);
                break;
            case MODE_MULTI_PAXOS:
                break;
            default:
                verify(0);
        }
        dtxn->frame_ = this;
        return dtxn;
    }

    Executor *Frame::CreateExecutor(cmdid_t cmd_id, Scheduler *sched) {
        Executor *exec = nullptr;
        auto mode = Config::GetConfig()->cc_mode_;
        switch (mode) {
            case MODE_NONE:
                verify(0);
            case MODE_2PL:
                exec = new TplExecutor(cmd_id, sched);
                break;
            case MODE_OCC:
                exec = new OCCExecutor(cmd_id, sched);
                break;
            case MODE_MDCC:
                exec = new mdcc::MdccExecutor(cmd_id, sched);
                break;
            default:
                verify(0);
        }
        return exec;
    }

    Scheduler *Frame::CreateScheduler() {
        auto mode = Config::GetConfig()->cc_mode_;
        Scheduler *sch = nullptr;
        switch (mode) {
            case MODE_2PL:
                sch = new TPLSched();
                break;
            case MODE_OCC:
                sch = new OCCSched();
                break;
            case MODE_MDCC:
                sch = new mdcc::MdccScheduler();
                break;
            case MODE_NONE:
                sch = new NoneSched();
                break;
            case MODE_RPC_NULL:
            case MODE_RCC:
            case MODE_RO6:
                verify(0);
                break;
            default:
                sch = new Scheduler(mode);
        }
        verify(sch);
        sch->frame_ = this;
        return sch;
    }

    TxnGenerator *Frame::CreateTxnGenerator(uint32_t partition_id) {
        auto benchmark = Config::config_s->benchmark_;
        TxnGenerator *gen = nullptr;
        switch (benchmark) {
            case TPCC:
                gen = new TpccTxnGenerator(Config::GetConfig(), partition_id);
                break;
            case CH_BENCHMARK:
                gen = new ChbenchmarkTxnGenerator(Config::GetConfig(), partition_id);
                break;
            case TPCC_DIST_PART:
            case TPCC_REAL_DIST_PART:
                gen = new TpccdTxnGenerator(Config::GetConfig());
                break;
            case TPCA:
                gen = new TpcaTxnGenerator(Config::GetConfig());
                break;
            case RW_BENCHMARK:
                gen = new RWTxnGenerator(Config::GetConfig());
                break;
            case RETWIS:
                gen = new RetwisTxnGenerator(Config::GetConfig());
                break;
            case MICRO_BENCH:
            default:
                gen = new TxnGenerator(Config::GetConfig());
        }
        return gen;
    }

    vector<rrr::Service *> Frame::CreateRpcServices(uint32_t site_id,
                                                    Scheduler *dtxn_sched,
                                                    rrr::PollMgr *poll_mgr,
                                                    ServerControlServiceImpl *scsi) {
        auto config = Config::GetConfig();
        auto result = std::vector<Service *>();
        switch (mode_) {
            case MODE_MDCC:
                result.push_back(new mdcc::MdccClientServiceImpl(config,
                                                                 site_id,
                                                                 dtxn_sched));
                result.push_back(new mdcc::MdccLeaderServiceImpl(config,
                                                                 site_id,
                                                                 dtxn_sched));
                result.push_back(new mdcc::MdccAcceptorServiceImpl(config,
                                                                   site_id,
                                                                   dtxn_sched));
                result.push_back(new mdcc::MdccLearnerServiceImpl(config,
                                                                  site_id,
                                                                  dtxn_sched));
                break;
            case MODE_2PL:
            case MODE_OCC:
            case MODE_NONE:
            case MODE_TAPIR:
            case MODE_JANUS:
            case MODE_RCC:
            case MODE_CHRONOS:
            case MODE_SLOG:
            case MODE_OV:
                result.push_back(new ClassicServiceImpl(dtxn_sched, poll_mgr, scsi));
                break;
            default:
                verify(0);
        }
        return result;
    }

    map<string, int> &Frame::FrameNameToMode() {
        static map<string, int> frame_name_mode_s = {
                {"none",          MODE_NONE},
                {"2pl",           MODE_2PL},
                {"chronos",       MODE_CHRONOS},
                {"slog",          MODE_SLOG},
                {"ov",            MODE_OV},
                {"occ",           MODE_OCC},
                {"ro6",           MODE_RO6},
                {"rpc_null",      MODE_RPC_NULL},
                {"deptran",       MODE_DEPTRAN},
                {"deptran_er",    MODE_DEPTRAN},
                {"2pl_w",         MODE_2PL},
                {"2pl_wait_die",  MODE_2PL},
                {"2pl_wd",        MODE_2PL},
                {"2pl_ww",        MODE_2PL},
                {"2pl_wound_die", MODE_2PL},
                {"mdcc",          MODE_MDCC},
                {"multi_paxos",   MODE_MULTI_PAXOS},
                {"epaxos",        MODE_NOT_READY},
                {"rep_commit",    MODE_NOT_READY}
        };
        return frame_name_mode_s;
    }

    map<int, function<Frame *()>> &Frame::ModeToFrame() {
        static map<int, function<Frame *()>> frame_s_ = {};
        return frame_s_;
    }
} // namespace rococo;
