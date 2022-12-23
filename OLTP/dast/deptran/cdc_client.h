//
// Created by micha on 2021/4/7.
//

#ifndef HTAP_CDC_CLIENT_H
#define HTAP_CDC_CLIENT_H

#endif//HTAP_CDC_CLIENT_H

#include "__dep__.h"

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

//TODO: This favors IDE, but is ugly
#include "../../build/state_transfer.grpc.pb.h"
#include "../../build/state_transfer.pb.h"

#include "../bench/chBenchmark/piece.h"


namespace rococo {


class CdcClient {
public:
    string server_address_ = "10.58.0.200:51209";

    CdcClient();


    parid_t my_par_id_;


    std::unique_ptr<state_transfer::OLAPServer::Stub> stub_;

    map<string, std::unique_ptr<grpc::ClientReaderWriter<state_transfer::UpdateReq, state_transfer::UpdateResp>>> col_update_streams_;
    map<string, grpc::ClientContext *> col_update_contexts_;
    map<string, vector<state_transfer::Update *>> col_update_buffers_;
    std::mutex update_mu_;

    std::unique_ptr<grpc::ClientReaderWriter<state_transfer::OrderReq, state_transfer::OrderResp>> order_stream_;
    grpc::ClientContext *order_context_;
    vector<state_transfer::Order *> order_buffer_;
    grpc::CompletionQueue *cq_;
    std::mutex order_mu_;
    grpc::Status order_stream_status_;
    const map<string, map<int, string>> col_id_name_map = {{"new_order", {{CHBENCHMARK_COL_NEW_ORDER_NO_O_ID, "new_order.no_o_id"}, {CHBENCHMARK_COL_NEW_ORDER_NO_D_ID, "new_order.no_d_id"}, {CHBENCHMARK_COL_NEW_ORDER_NO_W_ID, "new_order.no_w_id"}}},
                                                           {"district", {{CHBENCHMARK_COL_DISTRICT_D_NEXT_O_ID, "district.d_next_o_id"}, {CHBENCHMARK_COL_DISTRICT_D_YTD, "district.d_ytd"}}},
                                                           {"customer", {{CHBENCHMARK_COL_CUSTOMER_C_BALANCE, "customer.c_balance"}, {CHBENCHMARK_COL_CUSTOMER_C_YTD_PAYMENT, "customer.c_ytd_payment"}, {CHBENCHMARK_COL_CUSTOMER_C_DELIVERY_CNT, "customer.c_delivery_cnt"}, {CHBENCHMARK_COL_CUSTOMER_C_DATA, "customer.c_data"}}},
                                                           {"stock", {{CHBENCHMARK_COL_STOCK_S_QUANTITY, "stock.s_quantity"}, {CHBENCHMARK_COL_STOCK_S_YTD, "stock.s_ytd"}, {CHBENCHMARK_COL_STOCK_S_ORDER_CNT, "stock.s_order_cnt"}, {CHBENCHMARK_COL_STOCK_S_REMOTE_CNT, "stock.s_remote_cnt"}}},
                                                           {"order", {{CHBENCHMARK_COL_ORDER_O_ID, "order.o_id"}, {CHBENCHMARK_COL_ORDER_O_D_ID, "order.o_d_id"}, {CHBENCHMARK_COL_ORDER_O_W_ID, "order.o_w_id"}, {CHBENCHMARK_COL_ORDER_O_C_ID, "order.o_c_id"}, {CHBENCHMARK_COL_ORDER_O_ENTRY_D, "order.o_entry_d"}, {CHBENCHMARK_COL_ORDER_O_CARRIER_ID, "order.o_carrier_id"}, {CHBENCHMARK_COL_ORDER_O_OL_CNT, "order.o_ol_cnt"}, {CHBENCHMARK_COL_ORDER_O_ALL_LOCAL, "order.o_all_local"}}},
                                                           {"order_line", {{CHBENCHMARK_COL_ORDER_LINE_OL_O_ID, "order_line.ol_o_id"}, {CHBENCHMARK_COL_ORDER_LINE_OL_D_ID, "order_line.ol_d_id"}, {CHBENCHMARK_COL_ORDER_LINE_OL_W_ID, "order_line.ol_w_id"}, {CHBENCHMARK_COL_ORDER_LINE_OL_NUMBER, "order_line.ol_number"}, {CHBENCHMARK_COL_ORDER_LINE_OL_I_ID, "order_line.ol_i_id"}, {CHBENCHMARK_COL_ORDER_LINE_OL_SUPPLY_W_ID, "order_line.ol_supply_w_id"}, {CHBENCHMARK_COL_ORDER_LINE_OL_DELIVERY_D, "order_line.ol_delivery_d"}, {CHBENCHMARK_COL_ORDER_LINE_OL_QUANTITY, "order_line.ol_quantity"}, {CHBENCHMARK_COL_ORDER_LINE_OL_AMOUNT, "order_line.ol_amount"}, {CHBENCHMARK_COL_ORDER_LINE_OL_DIST_INFO, "order_line.ol_dist_info"}}},
                                                           {"history", {{CHBENCHMARK_COL_HISTORY_H_KEY, "history.h_key"}, {CHBENCHMARK_COL_HISTORY_H_C_ID, "history.h_c_id"}, {CHBENCHMARK_COL_HISTORY_H_C_D_ID, "history.h_c_d_id"}, {CHBENCHMARK_COL_HISTORY_H_C_W_ID, "history.h_c_w_id"}, {CHBENCHMARK_COL_HISTORY_H_D_ID, "history.h_d_id"}, {CHBENCHMARK_COL_HISTORY_H_W_ID, "history.h_w_id"}, {CHBENCHMARK_COL_HISTORY_H_DATE, "history.h_date"}, {CHBENCHMARK_COL_HISTORY_H_AMOUNT, "history.h_amount"}, {CHBENCHMARK_COL_HISTORY_H_DATA, "history.h_data"}}}};

    void TransferUpdate(int col_name,
                        const string &tbl_name,
                        const string &row_key,
                        const string &p_key_col_name,
                        int64_t rowIndex,
                        state_transfer::Update_OpType optype,
                        const string &col_value,
                        txnid_t txn_id);


    void TransferOrder(txnid_t txn_id,
                       vector<txnid_t> parents_tid,
                       txntype_t txn_type,
                       set<pair<string, int>> touched_cols,
                       vector<parid_t> touched_shards,
                       bool isQuery,
                       uint64_t estimated_time);


    string col_id_2_string(const string &tbl_name, int col_id);
};
}// namespace rococo
