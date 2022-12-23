

#include "sched.h"
#include "../rcc/dtxn.h"
#include "deptran/cdc_client.h"
#include "commo.h"
#include <bench/tpcc/piece.h>
#include "deptran/frame.h"

using namespace rococo;


map<txnid_t, RccDTxn *> BrqSched::Aggregate(RccGraph &graph) {
    // aggregate vertexes
    map<txnid_t, RccDTxn *> index;
    for (auto &pair : graph.vertex_index()) {
        RccDTxn *rhs_v = pair.second;
        verify(pair.first == rhs_v->id());
        RccDTxn *vertex = AggregateVertex(rhs_v);
        RccDTxn &dtxn = *vertex;
        if (dtxn.epoch_ == 0) {
            dtxn.epoch_ = epoch_mgr_.curr_epoch_;
        }
        epoch_mgr_.AddToEpoch(dtxn.epoch_, dtxn.tid_, dtxn.IsDecided());
        verify(vertex->id() == pair.second->id());
        verify(vertex_index().count(vertex->id()) > 0);
        index[vertex->id()] = vertex;
    }
    // aggregate edges.
    RebuildEdgePointer(index);

#ifdef DEBUG_CODE
    verify(index.size() == graph.vertex_index_.size());
    for (auto &pair : index) {
        txnid_t txnid = pair.first;
        RccVertex *v = pair.second;
        verify(v->parents_.size() == v->incoming_.size());
        auto sz = v->parents_.size();
        if (v->Get().status() >= TXN_CMT)
            RccSched::__DebugCheckParentSetSize(txnid, sz);
    }

    for (auto &pair : graph.vertex_index_) {
        auto txnid = pair.first;
        RccVertex *rhs_v = pair.second;
        auto lhs_v = FindV(txnid);
        verify(lhs_v != nullptr);
        // TODO, check the Sccs are the same.
        if (rhs_v->Get().status() >= TXN_DCD) {
            verify(lhs_v->Get().status() >= TXN_DCD);
            if (!AllAncCmt(rhs_v))
                continue;
            RccScc &rhs_scc = graph.FindSCC(rhs_v);
            for (RccVertex *rhs_vv : rhs_scc) {
                verify(rhs_vv->Get().status() >= TXN_DCD);
                RccVertex *lhs_vv = FindV(rhs_vv->id());
                verify(lhs_vv != nullptr);
                verify(lhs_vv->Get().status() >= TXN_DCD);
                verify(lhs_vv->GetParentSet() == rhs_vv->GetParentSet());
            }
            if (!AllAncCmt(lhs_v)) {
                continue;
            }
            RccScc &lhs_scc = FindSCC(lhs_v);
            for (RccVertex *lhs_vv : rhs_scc) {
                verify(lhs_vv->Get().status() >= TXN_DCD);
                RccVertex *rhs_vv = graph.FindV(lhs_v->id());
                verify(rhs_vv != nullptr);
                verify(rhs_vv->Get().status() >= TXN_DCD);
                verify(rhs_vv->GetParentSet() == rhs_vv->GetParentSet());
            }

            auto lhs_sz = lhs_scc.size();
            auto rhs_sz = rhs_scc.size();
            if (lhs_sz != rhs_sz) {
                // TODO
                for (auto &vv : rhs_scc) {
                    auto vvv = FindV(vv->id());
                    verify(vvv->Get().status() >= TXN_DCD);
                }
                verify(0);
            }
            //      verify(sz >= rhs_sz);
            //      verify(sz == rhs_sz);
            for (auto vv : rhs_scc) {
                bool r = std::any_of(lhs_scc.begin(),
                                     lhs_scc.end(),
                                     [vv](RccVertex *vvv) -> bool {
                                         return vvv->id() == vv->id();
                                     });
                verify(r);
            }
        }

        if (lhs_v->Get().status() >= TXN_DCD && AllAncCmt(lhs_v)) {
            RccScc &scc = FindSCC(lhs_v);
            for (auto vv : scc) {
                auto s = vv->Get().status();
                verify(s >= TXN_DCD);
            }
        }
    }

#endif
    //  this->BuildEdgePointer(graph, index);
    return index;
}


void BrqSched::OnPreAccept(const txnid_t txn_id,
                           const vector<SimpleCommand> &cmds,
                           const RccGraph &graph,
                           int32_t *res,
                           RccGraph *res_graph,
                           function<void()> callback) {
    verify(0);
}

//xs: Since we disable the "dispatch" phase, this is the only func that will be called for Pre-Accept
//hz: wo = without
void BrqSched::OnPreAcceptWoGraph(const txnid_t txn_id,
                                  const vector<SimpleCommand> &cmds,
                                  int32_t *res,
                                  RccGraph *res_graph,
                                  function<void()> callback) {
    std::lock_guard<std::recursive_mutex> lock(mtx_);
    Log_debug("on pre-accept wo graph: %lu, n cmds = %d", txn_id, cmds.size());
    //  Log_debug("on preaccept: %llx par: %d", txn_id, (int)partition_id_);
    //if (RandomGenerator::rand(1, 100) <= 1)
    //    Log_info("on pre-accept graph size: %d", graph_.size());
    verify(txn_id > 0);
    verify(cmds[0].root_id_ == txn_id);
    //  dep_graph_->Aggregate(const_cast<RccGraph&>(graph));
    //  TriggerCheckAfterAggregation(const_cast<RccGraph&>(graph));
    // TODO FIXME
    // add interference based on cmds.
    RccDTxn *dtxn = (RccDTxn *) GetOrCreateDTxn(txn_id); //AddV
    dtxn->root_type = cmds[0].root_type_;
    for(auto &c : cmds){
        dtxn->pieces_.push_back(c);
    }
    dtxn->touchColsCnt = cmds[0].touchColsCnt;
    dtxn->UpdateStatus(TXN_PAC);
    dtxn->involve_flag_ = RccDTxn::INVOLVED;
    RccDTxn &tinfo = *dtxn;
    if (dtxn->max_seen_ballot_ > 0) {
        *res = REJECT;
    } else {
        if (dtxn->status() < TXN_CMT) {
            if (dtxn->phase_ < PHASE_RCC_DISPATCH && tinfo.status() < TXN_CMT) {
                dtxn->phase_ = PHASE_RCC_DISPATCH;
                //                Log_debug("here");
                //need to execute to check dep
                //                        for (auto& c: cmds) {Vjjj
                //                          map<int32_t, Value> output;
                //                          dtxn->DispatchExecute(c, res, &output);
                //                        }

                //only work for TPCC
                bool input_not_ready = false;
                bool has_wait = false;
                for (auto &c : cmds) {
                    dtxn->dreqs_.push_back(c);
                    dtxn->n_local_pieces++;
                    Log_debug("received cmd %u, input ready ? %d", c.type_, c.input.piece_input_ready_);
                    //                    for (auto &pair : c.waiting_var_par_map) {
                    //                        for (auto &p : pair.second) {
                    //                            Log_debug("%d is required by par %u", pair.first, p);
                    //                        }
                    //                    }
                    if (c.type_ == TPCC_PAYMENT_4 || c.input.piece_input_ready_ == false){
                       input_not_ready = true;
                    }
                    if (c.type_ == TPCC_PAYMENT_5){
                        has_wait = true;
                    }
                }
                //TODO
                //if (has_wait == true && input_not_ready == true){
                    dtxn->TraceDepUnkKey(this->key_not_known_txns_);
                //}
                bool all_dep_checked = dtxn->DispatchExec(Scheduler::partition_id_);

                //TODO
                if (!all_dep_checked){
                    if (std::find(key_not_known_txns_.begin(), key_not_known_txns_.end(), dtxn) == key_not_known_txns_.end()){
                        //not in the vec
                        //key_not_known_txns_.push_back(dtxn);
                        Log_debug("insert key not known txn %lu, set size = %d", dtxn->id(), key_not_known_txns_.size());
                    }
                }
            }
        } else {
            if (dtxn->dreqs_.size() == 0) {
                for (auto &c : cmds) {
                    dtxn->dreqs_.push_back(c);
                    dtxn->n_local_pieces++;
                }
            }
        }
        verify(!tinfo.fully_dispatched);
        tinfo.fully_dispatched = true;
        Log_debug("Before MinItfrGraph, txn %lu's parent size =%d", tinfo.id(), tinfo.parents_.size());
        MinItfrGraph(dtxn, res_graph, false, 1);
        Log_debug("After MinItfrGraph, txn %lu's parent size =%d", tinfo.id(), tinfo.parents_.size());
        //Log_info("Txn size: %d, Graph size: %d", sizeof(*dtxn), sizeof(*res_graph));

        //Log_info("Local Map size %d", vertex_index().size());
        for (auto& pair: res_graph->vertex_index()){
            RccDTxn* rhs_v = pair.second;
            Log_debug("%s for txn %lu, rhs %lu, parents size = %d", __FUNCTION__ , tinfo.id(), rhs_v->id(), rhs_v->parents_.size());
        }


        if (tinfo.status() >= TXN_CMT) {
            waitlist_.insert(dtxn);
            verify(dtxn->epoch_ > 0);
        }
        *res = SUCCESS;
    }
    callback();
}

string GetTouchedTblCols(innid_t inn_id, parid_t par_id, map<parid_t, set<pair<string, int>>> &cols, int touchColsCnt, bool isQuery){
    string tbl_name = "";
    vector<string> touch_cols = {};
    innid_t normalized_inn_id = inn_id;
    int base = isQuery ? 1000: 100;

    // To normalize inn_id likes TPCC_NEW_ORDER_RI(i) to TPCC_NEW_ORDER_RI(0)
    if (normalized_inn_id >= 10*base) {
        while(normalized_inn_id >= base){
            normalized_inn_id /= 10;
        }
        normalized_inn_id *= 1000;
    }

    auto cols_iter = touch_col_map.find(normalized_inn_id);
    if (cols_iter != touch_col_map.end()) {
        touch_cols = cols_iter->second;
    }
    for (string touch_col: touch_cols) {
        bool flag = false;
        for (auto touch_col_iter = cols[par_id].begin(); touch_col_iter != cols[par_id].end(); touch_col_iter++) {
            if(touch_col == (*touch_col_iter).first ){
                int origin = (*touch_col_iter).second;
                cols[par_id].erase(touch_col_iter);
                origin ++;
                cols[par_id].insert(make_pair(touch_col, origin));
                flag = true;
                break;
            }
        }
        if (!flag){
            cols[par_id].insert(make_pair(touch_col, 1));
        }
    }

    auto tbl_iter = touch_table_map.find(normalized_inn_id);
    if (tbl_iter != touch_table_map.end()) {
        tbl_name = tbl_iter->second;
    }

    Log_debug("%s, piece type %d, normalized_inn_id %d, table name: %s", __FUNCTION__, inn_id, tbl_iter->first, tbl_name.c_str());

    return tbl_name;
}

void BrqSched::OnAccept(const txnid_t txn_id,
                        const ballot_t &ballot,
                        const RccGraph &graph,
                        int32_t *res,
                        function<void()> callback) {
    Log_debug("on accept: %lu", txn_id);
    std::lock_guard<std::recursive_mutex> lock(mtx_);
    RccDTxn *dtxn = (RccDTxn *) GetOrCreateDTxn(txn_id);
    if (dtxn->max_seen_ballot_ > ballot) {
        *res = REJECT;
        verify(0);// do not support failure recovery so far.
    } else {
        dtxn->max_accepted_ballot_ = ballot;
        dtxn->max_seen_ballot_ = ballot;
        //TODO transfer order
        verify(this->frame_ != nullptr);
        verify(this->frame_->cdc_client_ != nullptr);
        parid_t shard_id = this->frame_->site_info_->id / N_REP_PER_SHARD;
        if (this->frame_->site_info_->id % N_REP_PER_SHARD == 0) {
            //Log_info("Transfer order %lu at time %d", txn_id, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
            map<parid_t, set<pair<string, int>>> touched_cols;
            for (auto &p: dtxn->pieces_){
             //Get the txn info
             //return value not used
                string tbl_name = GetTouchedTblCols(p.inn_id(), p.PartitionId(), touched_cols, dtxn->touchColsCnt, false);
            }
            set<uint32_t>::iterator iter = dtxn->partition_.begin();
            while(iter != dtxn->partition_.end()){
               dtxn->all_touched_shards_.push_back(*iter);
               ++iter;
            }
            vector<txnid_t> parents_tid(dtxn->parents_.begin(),dtxn->parents_.end());
            this->frame_->cdc_client_->TransferOrder(txn_id, parents_tid , dtxn->root_type, touched_cols[shard_id], dtxn->all_touched_shards_, false, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() + 100);
        }
        Aggregate(const_cast<RccGraph &>(graph));
        *res = SUCCESS;
    }
    callback();
}

void BrqSched::OnSendOutput(const ChronosSendOutputReq &chr_req,
                            ChronosSendOutputRes *chr_res) {
    auto txn_id = chr_req.txn_id;
    Log_debug("%s called for txn %lu", __FUNCTION__, txn_id);
//    for (auto &pair: chr_req.var_values){
//        Log_debug("var %d received", pair.first);
//    }
    std::lock_guard<std::recursive_mutex> lock(mtx_);

    RccDTxn *dtxn = (RccDTxn *) GetOrCreateDTxn(txn_id);

    bool more_to_run = false;

    if (!dtxn->IsExecuted()) {//each output will be sent three times.
        //execution not finihsed yet.

        more_to_run = dtxn->MergeCheckReadyPieces(chr_req.var_values);
        if (more_to_run){
            auto v = FindV(txn_id);
            verify(v!= nullptr);
            waitlist_.insert(v);
            CheckWaitlist();
        }


    }
    Log_debug("%s returned for txn %lu, executed ? %d, more to run ? %d", __FUNCTION__, txn_id, dtxn->IsExecuted(), more_to_run);
}


//
//void BrqSched::OnCommit(const txnid_t cmd_id,
//                        const RccGraph& graph,
//                        int32_t* res,
//                        TxnOutput* output,
//                        const function<void()>& callback) {
//  // TODO to support cascade abort
//  std::lock_guard<std::recursive_mutex> lock(mtx_);
////  if (RandomGenerator::rand(1, 2000) <= 1)
////    Log_debug("on commit graph size: %d", graph.size());
//  *res = SUCCESS;
//  // union the graph into dep graph
//  RccDTxn *dtxn = (RccDTxn*) GetOrCreateDTxn(cmd_id);
//  verify(dtxn != nullptr);
//  RccDTxn& info = *dtxn;
//
//  verify(dtxn->ptr_output_repy_ == nullptr);
//  dtxn->ptr_output_repy_ = output;
//
//  if (info.IsExecuted()) {
//    verify(info.status() >= TXN_DCD);
//    verify(info.graphs_for_inquire_.size() == 0);
//    *res = SUCCESS;
//    callback();
//  } else if (info.IsAborted()) {
//    verify(0);
//    *res = REJECT;
//    callback();
//  } else {
////    Log_debug("on commit: %llx par: %d", cmd_id, (int)partition_id_);
//    dtxn->commit_request_received_ = true;
//    dtxn->finish_reply_callback_ = [callback, res] (int r) {
//      *res = r;
////      verify(r == SUCCESS);
//      callback();
//    };
//    auto index = Aggregate(const_cast<RccGraph&> (graph));
//    for (auto& pair: index) {
//      verify(pair.second->epoch_ > 0);
//    }
//    TriggerCheckAfterAggregation(const_cast<RccGraph &>(graph));
//    // fast path without check wait list?
////    if (graph.size() == 1) {
////      auto v = dep_graph_->FindV(cmd_id);
////      if (v->incoming_.size() == 0);
////      CheckInquired(v->Get());
////      Execute(v->Get());
////      return;
////    } else {
////      Log_debug("graph size on commit, %d", (int) graph.size());
//////    verify(0);
////    }
//  }
//
//}

//
//void BrqSched::OnCommitWoGraph(const txnid_t cmd_id,
//                               int32_t* res,
//                               TxnOutput* output,
//                               const function<void()>& callback) {
//  // TODO to support cascade abort
//  std::lock_guard<std::recursive_mutex> lock(mtx_);
//  *res = SUCCESS;
//  // union the graph into dep graph
//  RccDTxn *dtxn = (RccDTxn*) GetOrCreateDTxn(cmd_id);
//  verify(dtxn != nullptr);
//  RccDTxn& info = *dtxn;
//
//  verify(dtxn->ptr_output_repy_ == nullptr);
//  dtxn->ptr_output_repy_ = output;
//
//  if (info.IsExecuted()) {
//    verify(info.status() >= TXN_DCD);
//    verify(info.graphs_for_inquire_.size() == 0);
//    *res = SUCCESS;
//    callback();
//  } else if (info.IsAborted()) {
//    verify(0);
//    *res = REJECT;
//    callback();
//  } else {
////    Log_debug("on commit: %llx par: %d", cmd_id, (int)partition_id_);
//    dtxn->commit_request_received_ = true;
//    dtxn->finish_reply_callback_ = [callback, res] (int r) {
//      *res = r;
////      verify(r == SUCCESS);
//      callback();
//    };
//    UpgradeStatus(dtxn, TXN_CMT);
//    waitlist_.insert(dtxn);
//    verify(dtxn->epoch_ > 0);
//    CheckWaitlist();
//  }
//}

//why there is abort
//due to failure recovery
void BrqSched::OnCommit(const txnid_t cmd_id,
                        RccGraph *graph,
                        int32_t fast_path_,
                        int32_t *res,
                        TxnOutput *output,
                        const function<void()> &callback) {
    // TODO to support cascade abort
    std::lock_guard<std::recursive_mutex> lock(mtx_);
    Log_debug("%s called txnid %lu", __FUNCTION__, cmd_id);
    //  if (RandomGenerator::rand(1, 2000) <= 1)
    //    Log_debug("on commit graph size: %d", graph.size());
    *res = SUCCESS;
    // union the graph into dep graph
    RccDTxn *dtxn = (RccDTxn *) GetOrCreateDTxn(cmd_id);
    //  verify(dtxn != nullptr);
    verify(dtxn->ptr_output_repy_ == nullptr);
    dtxn->ptr_output_repy_ = output;
    //TODO transfer order
    if(this->frame_->site_info_->id % N_REP_PER_SHARD == 0){
        verify(this->frame_ != nullptr);
        verify(this->frame_->cdc_client_ != nullptr);
        parid_t shard_id = this->frame_->site_info_->id / N_REP_PER_SHARD;
        map<parid_t, set<pair<string, int>>> touched_cols;
        for (auto &p: dtxn->pieces_){
            //Get the txn info
            //return value not used
            string tbl_name = GetTouchedTblCols(p.inn_id(), p.PartitionId(), touched_cols, dtxn->touchColsCnt, false);
        }
        set<uint32_t>::iterator iter = dtxn->partition_.begin();
        while(iter != dtxn->partition_.end()){
           dtxn->all_touched_shards_.push_back(*iter);
            ++iter;
        }
        vector<txnid_t> parents_tid(dtxn->parents_.begin(),dtxn->parents_.end());
        this->frame_->cdc_client_->TransferOrder(dtxn->id(), parents_tid , dtxn->root_type, touched_cols[shard_id], dtxn->all_touched_shards_, false, std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    }
    if (dtxn->IsExecuted()) {
        //    verify(info.status() >= TXN_DCD);
        //    verify(info.graphs_for_inquire_.size() == 0);
        Log_debug("%s, already executed", __FUNCTION__);
        *res = SUCCESS;
        callback();
    } else if (dtxn->IsAborted()) {
        verify(0);
        *res = REJECT;
        callback();
    } else {
        //    Log_debug("on commit: %llx par: %d", cmd_id, (int)partition_id_);
        Log_debug("%s, going to exe", __FUNCTION__);
        dtxn->commit_request_received_ = true;
        dtxn->finish_reply_callback_ = [callback, res](int r) {
            *res = r;
            //      verify(r == SUCCESS);
            callback();
        };
        if (graph == nullptr) {
            // quick path without graph, no contention.
            verify(dtxn->fully_dispatched);//cannot handle non-dispatched now.
            UpgradeStatus(dtxn, TXN_DCD);
            TryExecReadyPieces(*dtxn, false);
            if (dtxn->IsExecuted()){
                auto it = std::find(key_not_known_txns_.begin(), key_not_known_txns_.end(), dtxn);
                if (it != key_not_known_txns_.end()){
                    key_not_known_txns_.erase(it);
                    Log_debug("removing txn %lu from key not knwon txns, size %lu", dtxn->id(), key_not_known_txns_.size());
                }
            }
            if (dtxn->to_checks_.size() > 0) {
                for (auto child : dtxn->to_checks_) {
                    waitlist_.insert(child);
                }
                CheckWaitlist();
            }
        } else {
            //for fast path graph
            Log_debug("Before aggregrated, txn %lu's parent size =%d", dtxn->id(), dtxn->parents_.size());
            auto index = Aggregate(*graph);
            Log_debug("After aggregrated, txn %lu's parent size =%d", dtxn->id(), dtxn->parents_.size());
            //      for (auto& pair: index) {
            //        verify(pair.second->epoch_ > 0);
            //      }
            TriggerCheckAfterAggregation(*graph);
        }

        // fast path without check wait list?
        //    if (graph.size() == 1) {
        //      auto v = dep_graph_->FindV(cmd_id);
        //      if (v->incoming_.size() == 0);
        //      CheckInquired(v->Get());
        //      Execute(v->Get());
        //      return;
        //    } else {
        //      Log_debug("graph size on commit, %d", (int) graph.size());
        ////    verify(0);
        //    }
    }
}


int BrqSched::OnInquire(epoch_t epoch,
                        cmdid_t cmd_id,
                        RccGraph *graph,
                        const function<void()> &callback) {
    std::lock_guard<std::recursive_mutex> lock(mtx_);
    // TODO check epoch, cannot be a too old one.
    RccDTxn *dtxn = (RccDTxn *) GetOrCreateDTxn(cmd_id);
    RccDTxn &info = *dtxn;
    //register an event, triggered when the status >= COMMITTING;
    verify(info.Involve(Scheduler::partition_id_));

    auto cb_wrapper = [callback, graph]() {
#ifdef DEBUG_CODE
        for (auto pair : graph->vertex_index_) {
            RccVertex *v = pair.second;
            TxnInfo &tinfo = v->Get();
            if (tinfo.status() >= TXN_CMT) {
                //        Log_debug("inquire ack, txnid: %llx, parent size: %d",
                //                 pair.first, v->GetParentSet().size());
                RccSched::__DebugCheckParentSetSize(v->id(), v->parents_.size());
            }
        }
#endif
        callback();
    };

    if (info.status() >= TXN_CMT) {
        InquiredGraph(info, graph);
        cb_wrapper();
    } else {
        info.graphs_for_inquire_.push_back(graph);
        info.callbacks_for_inquire_.push_back(cb_wrapper);
        verify(info.graphs_for_inquire_.size() ==
               info.callbacks_for_inquire_.size());
        waitlist_.insert(dtxn);
        verify(dtxn->epoch_ > 0);
    }
}

BrqCommo *BrqSched::commo() {

    auto commo = dynamic_cast<BrqCommo *>(commo_);
    verify(commo != nullptr);
    return commo;
}
