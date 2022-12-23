//
// Created by micha on 2021/2/19.
//

#ifndef ROCOCO_SLOG_MANAGER_H
#define ROCOCO_SLOG_MANAGER_H


#include "deptran/__dep__.h"
#include "deptran/slog/scheduler.h"
#include "deptran/rcc_rpc.h"
#include "deptran/slog/commo.h"


namespace rococo{

class SlogRegionManager: public SlogRegionMangerService, public SchedulerSlog{
public:
    SlogRegionManager(Frame *f);

    void OnSubmitIRT(const std::map<parid_t, std::vector<SimpleCommand>> &);


    // void SlogSendOrderedCRT(const map<parid_t, vector<SimpleCommand>>& cmd_by_par, const uint64_t& index, const siteid_t& handler_site, vector<parid_t>& touched_all_pars, rrr::i32* res, rrr::DeferredReply* defer) override;

    void SlogSendOrderedCRT(const std::map<parid_t, std::vector<SimpleCommand>>& cmds_by_par, const uint64_t& index, const siteid_t& handler_site, const std::vector<parid_t>& all_touched_shards, const std::vector<uint64_t>& barriers,  rrr::i32* res, rrr::DeferredReply* defer) override;


    void SendRmIRT(const std::map<uint32_t, std::vector<SimpleCommand>>& cmds_by_par, const siteid_t& handler_site, rrr::i32* res, rrr::DeferredReply* defer) override;

    //return the inserted index
    uint64_t InsertLocalLog(uint64_t global_index, const std::map<uint32_t, std::vector<SimpleCommand>>& cmds_by_par, const siteid_t& handler_site, const vector<parid_t>& touched_all_shards);

    void RmReplicate(const std::map<uint32_t, std::vector<SimpleCommand>>& cmds_by_par, const siteid_t& handler_site, rrr::i32* res, rrr::DeferredReply* defer) override;

    void ReplicateACK(size_t index);

    void CheckAndSendBacth();
    std::thread send_batch_thread_;

    struct logEntry{
        std::map<parid_t, std::vector<SimpleCommand>> cmds_;
        siteid_t coord_site_;
        bool committed_ = false;

        //note that this contains the touched pars that are not in my region
        //while cmds only have touched pars in my region.
        vector<parid_t> touched_pars_all_;
    };

    std::vector<logEntry* > log_;
    size_t next_commit_index_ = 0;
    size_t next_send_index_ = 0;


    std::vector<logEntry*> global_log_;

    size_t global_next_insert_index_ = 0;

    //use set for efficient lookup
    set<uint64_t> barrier_global_indices_;

    vector<uint64_t> barrier_local_indices_;
    uint64_t next_send_barrier_local_ = 0;

};


}


#endif//ROCOCO_SLOG_MANAGER_H
