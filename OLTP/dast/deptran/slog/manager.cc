//
// Created by micha on 2021/2/19.
//

#include "deptran/slog/manager.h"
#include "deptran/frame.h"
namespace rococo {


SlogRegionManager::SlogRegionManager(Frame *f) : SlogRegionMangerService(), SchedulerSlog(f) {
    this->my_rid_ = f->site_info_->id / (N_REP_PER_SHARD * Config::GetConfig()->n_shard_per_region_);
    Log_info("[Slog region manager] created for rid %u", my_rid_);

    if (f->site_info_->id % (Config::GetConfig()->n_shard_per_region_ * N_REP_PER_SHARD) == 0) {
        send_batch_thread_ = std::thread(&SlogRegionManager::CheckAndSendBacth, this);
        Log_debug("[Send batch thread created, interval =%d ms", batch_interval_);
    }
}

void SlogRegionManager::SendRmIRT(const std::map<uint32_t, std::vector<SimpleCommand>> &cmds_by_par, const uint16_t &handler_site, rrr::i32 *res, rrr::DeferredReply *defer) {


    std::lock_guard<std::recursive_mutex> guard(mtx_);
    //save to log
    //    logEntry *e = new logEntry();
    //
    //    e->cmds_ = cmds_by_par;
    //    e->committed_ = false;
    //    e->coord_site_ = handler_site;
    //    log_.push_back(e);
    //    size_t index = log_.size() - 1;
    //
    //
    //    //replicate
    //    std::function<void(Future *)> cb =
    //            [=](Future *fu) {
    //                int ack_res;
    //                fu->get_reply() >> ack_res;
    //                //notify all sites in my region.
    //                ReplicateACK(index);
    //            };
    //
    //    rrr::FutureAttr fuattr;
    //    fuattr.callback = cb;
    //
    //    //Find the backups
    //
    //    regionid_t rid = partition_id_ / Config::GetConfig()->n_shard_per_region_;
    //    siteid_t backup_site = rid * Config::GetConfig()->n_shard_per_region_ * N_REP_PER_SHARD + 1;
    //    commo()->slog_rpc_region_managers_.count(backup_site) != 0;
    //    auto proxy = commo()->slog_rpc_region_managers_[backup_site];
    //
    //    Future::safe_release(proxy->async_RmReplicate(cmds_by_par, handler_site, fuattr));
    vector<parid_t> touched_shards_all; 
    for (auto &pair: cmds_by_par){
        touched_shards_all.push_back(pair.first);
    }
    //global index == -1
    InsertLocalLog(-1, cmds_by_par, handler_site, touched_shards_all);

    defer->reply();
}

void SlogRegionManager::RmReplicate(const std::map<uint32_t, std::vector<SimpleCommand>> &cmds_by_par, const siteid_t &handler_site, rrr::i32 *res, rrr::DeferredReply *defer) {
    Log_debug("[Slog region manager] %s called for txn %lu", __FUNCTION__, cmds_by_par.begin()->second.at(0).id_);
    std::lock_guard<std::recursive_mutex> guard(mtx_);

    logEntry *e = new logEntry();
    e->cmds_ = cmds_by_par;
    e->committed_ = false;
    log_.push_back(e);

    defer->reply();
}

void SlogRegionManager::ReplicateACK(size_t index) {
    Log_debug("[Slog region manager] %s called", __FUNCTION__);
    std::lock_guard<std::recursive_mutex> guard(mtx_);

    verify(log_.size() > index);

    log_[index]->committed_ = true;

    for (size_t i = next_commit_index_; i < log_.size(); i++) {
        if (log_[i]->committed_) {
            next_commit_index_++;
        } else {
            break;
        }
    }
}

void SlogRegionManager::CheckAndSendBacth() {
    while (true) {
        std::unique_lock<std::recursive_mutex> lk(mtx_);
        if (commo_ != nullptr && next_send_index_ < next_commit_index_) {
            parid_t start_par = my_rid_ * (Config::GetConfig()->n_shard_per_region_);
            parid_t end_par = (my_rid_ + 1) * (Config::GetConfig()->n_shard_per_region_);
            for (parid_t target_par = start_par; target_par < end_par; target_par++) {
                uint64_t start_index = next_send_index_;
                vector<pair<txnid_t, vector<SimpleCommand>>> cmds;
                vector<pair<txnid_t, siteid_t>> coord_sites;
                vector<pair<txnid_t, vector<parid_t>>> touched_pars;
                vector<pair<txnid_t, i32>> isQuery;
                vector<pair<txnid_t, i32>> touchColsCnt;
                vector<pair<txnid_t, cmdtype_t>> txn_type;

                for (auto i = next_send_index_; i < next_commit_index_; i++) {
                    std::pair<txnid_t, vector<SimpleCommand>> txn;
                    txnid_t txn_id = log_[i]->cmds_.begin()->second.at(0).root_id_;
                    txn.first = txn_id;
                    if (log_[i]->cmds_.count(target_par) != 0) {
                        txn.second = log_[i]->cmds_.at(target_par);
                    }
                    cmds.push_back(txn);
                    coord_sites.emplace_back(std::pair<txnid_t, siteid_t>(txn_id, log_[i]->coord_site_));
                    // Log_info("[manaer here], txn %lu touched pars size %d", txn_id, log_[i]->touched_pars_all_.size());
                    touched_pars.emplace_back(std::pair<txnid_t, vector<parid_t>>(txn_id, log_[i]->touched_pars_all_));
//                    Log_info("[manaer here], txn %lu isQuery %d", txn_id, (i32)log_[i]->cmds_.begin()->second.at(0).isQuery);
                    isQuery.emplace_back(std::pair<txnid_t, i32>(txn_id, (i32)log_[i]->cmds_.begin()->second.at(0).isQuery));
//                    Log_info("[manaer here], txn %lu touchColsCnt %d", txn_id, (i32)log_[i]->cmds_.begin()->second.at(0).touchColsCnt);
                    touchColsCnt.emplace_back(std::pair<txnid_t, i32>(txn_id, (i32)log_[i]->cmds_.begin()->second.at(0).touchColsCnt));
                    txn_type.emplace_back(std::pair<txnid_t, cmdtype_t>(txn_id, log_[i]->cmds_.begin()->second.at(0).txn_type));
                }
                verify(cmds.size() == coord_sites.size());
                auto proxies = commo()->rpc_par_proxies_[target_par];
                
                // for (auto &pair : touched_pars) {
                //     Log_info("[manager before send batch] txn %lu", pair.first);
                //     for (auto &itr : pair.second) {
                //         Log_info("touched %d", itr);
                //     }
                // }

                vector<uint64_t> barriers;
                while(next_send_barrier_local_ < barrier_local_indices_.size()){
                    barriers.push_back(barrier_local_indices_[next_send_barrier_local_]);
                    next_send_barrier_local_++;
                }

                for (auto &p : proxies) {
                    Log_debug("[%s] sending to site %hu of par %u of %lu txns, start index = %d", __FUNCTION__, p.first, target_par, cmds.size(), start_index);


                    //TODO
                    Future::safe_release(p.second->async_SlogSendBatch(start_index, cmds, coord_sites, touched_pars, barriers, isQuery, touchColsCnt, txn_type));
                }
            }
            next_send_index_ = next_commit_index_;
        }
        lk.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(batch_interval_));
    }
}

//OnSlogSendOrderedCRT.
void SlogRegionManager::SlogSendOrderedCRT(const map<parid_t,
                                                     vector<SimpleCommand>> &cmd_by_par,
                                           const uint64_t &index,
                                           const siteid_t &handler_site,
                                           const vector<parid_t> &touched_all_pars,
                                           const vector<uint64_t> &barrier_global_indices,
                                           rrr::i32 *res, rrr::DeferredReply *defer) {
    if (cmd_by_par.size() != 0) {
        Log_debug("[Slog region manager] %s called for index %lu txn id = %lu, touched pars size = %d", __FUNCTION__, index, cmd_by_par.begin()->second.at(0).id_, touched_all_pars.size());
    } else {
        Log_debug("[Slog region manager] %s called for index %lu txn id = irrelevant", __FUNCTION__, index);
    }
    std::lock_guard<std::recursive_mutex> guard(mtx_);

    for (auto i = global_log_.size(); i < index + 1; i++) {
        global_log_.push_back(nullptr);
    }

    for (auto i: barrier_global_indices){
        this->barrier_global_indices_.insert(i);
        Log_info("received barrier at global index %lu", i);
    }

    verify(global_log_.size() > index);
    verify(global_log_[index] == nullptr);

    auto e = new logEntry;
    e->cmds_ = cmd_by_par;
    e->coord_site_ = handler_site;
    e->committed_ = false;
    e->touched_pars_all_ = vector<parid_t>(touched_all_pars);

    //todo touched

    // if (cmd_by_par.size() != 0){
    //     auto txn_id = (*cmd_by_par.begin()).second[0].root_id_;
    //     Log_info("[167] txn %lu touched %d shards", txn_id, e->touched_pars_all_.size());
    // }

    global_log_[index] = e;


    for (auto i = global_next_insert_index_; i < global_log_.size(); i++) {
        if (global_log_[i] == nullptr) {
            break;
        }
        //insert global_log_[i] into local log
        if (global_log_[i]->cmds_.size() != 0) {
            Log_debug("[Slog region manager] inserting global log index %d into local log, txn id = %lu", i, global_log_[i]->cmds_.begin()->second.at(0).id_);
            InsertLocalLog(i, global_log_[i]->cmds_, global_log_[i]->coord_site_, touched_all_pars);


        } else {
            Log_debug("[Slog region manager] skiping global log index %d into local log, irrelevant", i);
        }
        global_log_[i]->committed_ = true;
        global_next_insert_index_++;
    }
}

uint64_t SlogRegionManager::InsertLocalLog(uint64_t global_index,
                                           const std::map<uint32_t, std::vector<SimpleCommand>> &cmds_by_par, const uint16_t &handler_site,
                                           const vector<parid_t> &touched_all_shards) {

    std::lock_guard<std::recursive_mutex> guard(mtx_);



    //save to log
    logEntry *e = new logEntry();

    e->cmds_ = cmds_by_par;
    e->committed_ = false;
    e->coord_site_ = handler_site;
    e->touched_pars_all_ = touched_all_shards;

    // for (auto& pair: cmds_by_par){
    //     Log_info("par %u touched by txn %lu", pair.first, pair.second[0].root_id_);
    // }

    log_.push_back(e);
    size_t index = log_.size() - 1;

    if (barrier_global_indices_.count(global_index) != 0){   
        Log_info("barrier at global index %lu mapped to local index %lu", global_index, index);
        barrier_local_indices_.push_back(index); 
    }

    //replicate
    std::function<void(Future *)> cb =
            [=](Future *fu) {
                int ack_res;
                fu->get_reply() >> ack_res;
                //notify all sites in my region.
                ReplicateACK(index);
            };

    rrr::FutureAttr fuattr;
    fuattr.callback = cb;

    //Find the backups

    regionid_t rid = partition_id_ / Config::GetConfig()->n_shard_per_region_;
    siteid_t backup_site = rid * Config::GetConfig()->n_shard_per_region_ * N_REP_PER_SHARD + 1;
    commo()->slog_rpc_region_managers_.count(backup_site) != 0;
    auto proxy = commo()->slog_rpc_region_managers_[backup_site];

    Future::safe_release(proxy->async_RmReplicate(cmds_by_par, handler_site, fuattr));

    Log_debug("[Slog region manager] %s called for txn %lu to backup site %hu", __FUNCTION__, cmds_by_par.begin()->second.at(0).id_, backup_site);

    return index;
}


}//namespace rococo