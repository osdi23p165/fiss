//
// Created by micha on 2021/4/7.
//


#include "deptran/cdc_client.h"


namespace rococo {

CdcClient::CdcClient() {
    auto channel = grpc::CreateChannel(server_address_, grpc::InsecureChannelCredentials());

    stub_ = state_transfer::OLAPServer::NewStub(channel);

    printf("CDC Client created, server address = %s\n", server_address_.c_str());
}

void CdcClient::TransferOrder(txnid_t txn_id,
                              vector<txnid_t> parents_tid,
                              txntype_t txn_type,
                              set<pair<string, int>> touched_cols,
                              vector<parid_t> touched_shards,
                              bool isQuery,
                              uint64_t estimated_time) {


    //return;

    string touched_str = "";
    for (auto &par : touched_shards) {
        touched_str += std::to_string(par) + ",";
    }
    std::lock_guard<std::mutex> mu(order_mu_);


    state_transfer::Order *od = new state_transfer::Order();
    od->set_txn_id(txn_id);
    od->set_txn_type(txn_type);
    //od->set_isquery(isQuery);
    auto touch_cols = od->mutable_touched_cols();
    for (auto touch_col_iter = touched_cols.begin(); touch_col_iter != touched_cols.end(); touch_col_iter++) {
        state_transfer::TouchedColumn *touch_col = new state_transfer::TouchedColumn();
        touch_col->set_col_id((*touch_col_iter).first);
        touch_col->set_up_num((*touch_col_iter).second);
        touch_cols->AddAllocated(touch_col);
    }
    for (auto pid : parents_tid) {
        od->add_parents_tid(pid);
    }
    for (auto parid : touched_shards) {
        od->add_touched_shard(parid);
    }

    //put into buffer, wait for forming batches
    order_buffer_.push_back(od);

    //form batch
    //Log_info("%s called, txn_id = %lu, touched_shards = %s, isQuery = %d", __FUNCTION__, txn_id, touched_str.c_str(), isQuery);
    if (order_buffer_.size() >= 100) {
        //Log_info("Transfer Order form batches, my_parid= %d, batch size =%d",
        // my_par_id_,
        // order_buffer_.size());

        if (order_stream_ == nullptr) {
            order_context_ = new grpc::ClientContext();
            order_stream_ = stub_->TransferOrder(order_context_);
        }

        state_transfer::OrderReq req;
        req.set_shard_id(my_par_id_);
        auto ods = req.mutable_orders();
        for (auto od : order_buffer_) {
            od->set_estimated_lat(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
            ods->AddAllocated(od);
        }
        order_buffer_.clear();
        order_stream_->Write(req);
        //Log_info("Transferred Order of par %d", my_par_id_);
    }
}

void CdcClient::TransferUpdate(int col_name,
                               const string &tbl_name,
                               const string &row_key,
                               const string &p_key_col_name,
                               int64_t rowIndex,
                               state_transfer::Update_OpType optype,
                               const string &col_value,
                               txnid_t txn_id) {


    //return;
    std::lock_guard<std::mutex> mu(update_mu_);

    //do a batching here

    //not necessary
    // if (col_update_buffers_.count(col_name) == 0){
    //     col_update_buffers_[col_name] = vector<state_transfer> update();
    // }
    auto up = new state_transfer::Update;
    up->set_row_index(rowIndex);
    up->set_pkey_col_name(p_key_col_name);
    up->set_pkey_val(row_key);
    up->set_value(col_value);
    up->set_op_type(optype);
    up->set_txnid(txn_id);
    string col_name_s = tbl_name + std::to_string(col_name);
    col_update_buffers_[col_name_s].push_back(up);
    //Log_info("%s called, transfer update txn_id: %lu, col: %s", __FUNCTION__, txn_id, col_id_2_string(tbl_name, col_name).c_str());
    //if size > 100,
    //TODO: add timing
    //batch cut criteria
    if (col_update_buffers_[col_name_s].size() > 100) {
        //Log_info("batch size for %d is %d, form batch", col_name, col_update_buffers_[col_name_s].size());

        //create the grpc client side if does not exist yet
        if (col_update_streams_.count(col_name_s) == 0) {
            auto context = new grpc::ClientContext;
            auto stream = stub_->TransferUpdate(context);

            col_update_contexts_[col_name_s] = context;
            col_update_streams_[col_name_s] = std::move(stream);
        }


        state_transfer::UpdateReq req;
        req.set_col_id(col_id_2_string(tbl_name, col_name));
        req.set_shard_id(my_par_id_);

        auto updates = req.mutable_updates();

        for (auto up : col_update_buffers_[col_name_s]) {
            updates->AddAllocated(up);
        }

        col_update_buffers_[col_name_s].clear();

        col_update_streams_[col_name_s]->Write(req);

        //Log_info("Transferred update of col %d", col_name);
    }
}

string CdcClient::col_id_2_string(const string &tbl_name, int col_id) {
    map<int, string> tbl_col_map = {};

    auto tbl_col_iter = col_id_name_map.find(tbl_name);
    if (tbl_col_iter == col_id_name_map.end()) {
        Log_info("Table %s does not exist", tbl_name.c_str());
    }
    tbl_col_map = tbl_col_iter->second;

    auto col_iter = tbl_col_map.find(col_id);
    if (col_iter == tbl_col_map.end()) {
        Log_info("Col %d does not exist in table %s", col_id, tbl_name.c_str());
    }
    return col_iter->second;
}

}// namespace rococo