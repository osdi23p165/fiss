
#pragma once
#include "../rcc/sched.h"
#include "bench/chBenchmark/piece.h"
#include "rcc_rpc.h"
namespace rococo {
const map<innid_t, string> touch_table_map = {{CHBENCHMARK_NEW_ORDER_0, "district"},
                                              {CHBENCHMARK_NEW_ORDER_1, ""},
                                              {CHBENCHMARK_NEW_ORDER_2, ""},
                                              {CHBENCHMARK_NEW_ORDER_3, "order"},
                                              {CHBENCHMARK_NEW_ORDER_4, "new_order"},
                                              {CHBENCHMARK_NEW_ORDER_RI(0), ""},
                                              {CHBENCHMARK_NEW_ORDER_RS(0), ""},
                                              {CHBENCHMARK_NEW_ORDER_WS(0), "stock"},
                                              {CHBENCHMARK_NEW_ORDER_WOL(0), "order_line"},

                                              {CHBENCHMARK_PAYMENT_0, ""},
                                              {CHBENCHMARK_PAYMENT_1, ""},
                                              {CHBENCHMARK_PAYMENT_2, "district"},
                                              {CHBENCHMARK_PAYMENT_3, ""},
                                              {CHBENCHMARK_PAYMENT_4, "customer"},
                                              {CHBENCHMARK_PAYMENT_5, "history"},

                                              {CHBENCHMARK_ORDER_STATUS_0, ""},
                                              {CHBENCHMARK_ORDER_STATUS_1, ""},
                                              {CHBENCHMARK_ORDER_STATUS_2, ""},
                                              {CHBENCHMARK_ORDER_STATUS_3, ""},

                                              {CHBENCHMARK_DELIVERY_0, ""},
                                              {CHBENCHMARK_DELIVERY_1, "order"},
                                              {CHBENCHMARK_DELIVERY_2, "order_line"},
                                              {CHBENCHMARK_DELIVERY_3, "customer"},

                                              {CHBENCHMARK_STOCK_LEVEL_0, ""},
                                              {CHBENCHMARK_STOCK_LEVEL_1, ""},
                                              {CHBENCHMARK_STOCK_LEVEL_RS(0), ""},

                                              {CHBENCHMARK_QUERY1_OL(0), "order_line"},

                                              {CHBENCHMARK_QUERY2_ST(0), "stock"},
                                              {CHBENCHMARK_QUERY2_SP(0), "supplier"},
                                              {CHBENCHMARK_QUERY2_IT(0), "item"},
                                              {CHBENCHMARK_QUERY2_NT(0), "nation"},
                                              {CHBENCHMARK_QUERY2_RG(0), "region"},

                                              {CHBENCHMARK_QUERY3_CT(0), "customer"},
                                              {CHBENCHMARK_QUERY3_NO(0), "new_order"},
                                              {CHBENCHMARK_QUERY3_OD(0), "order"},
                                              {CHBENCHMARK_QUERY3_OL(0), "order_line"},

                                              {CHBENCHMARK_QUERY4_OD(0), "order"},
                                              {CHBENCHMARK_QUERY4_OL(0), "order_line"},

                                              {CHBENCHMARK_QUERY5_CT(0), "customer"},
                                              {CHBENCHMARK_QUERY5_OD(0), "order"},
                                              {CHBENCHMARK_QUERY5_OL(0), "order_line"},
                                              {CHBENCHMARK_QUERY5_ST(0), "stock"},
                                              {CHBENCHMARK_QUERY5_SP(0), "supplier"},
                                              {CHBENCHMARK_QUERY5_NT(0), "nation"},
                                              {CHBENCHMARK_QUERY5_RG(0), "region"},

                                              {CHBENCHMARK_QUERY6_OL(0), "order_line"},

                                              {CHBENCHMARK_QUERY7_SP(0), "supplier"},
                                              {CHBENCHMARK_QUERY7_ST(0), "stock"},
                                              {CHBENCHMARK_QUERY7_OL(0), "order_line"},
                                              {CHBENCHMARK_QUERY7_OD(0), "order"},
                                              {CHBENCHMARK_QUERY7_CT(0), "customer"},
                                              {CHBENCHMARK_QUERY7_NT(0), "nation"},

                                              {CHBENCHMARK_QUERY8_IT(0), "item"},
                                              {CHBENCHMARK_QUERY8_SP(0), "supplier"},
                                              {CHBENCHMARK_QUERY8_ST(0), "stock"},
                                              {CHBENCHMARK_QUERY8_OL(0), "order_line"},
                                              {CHBENCHMARK_QUERY8_OD(0), "order"},
                                              {CHBENCHMARK_QUERY8_CT(0), "customer"},
                                              {CHBENCHMARK_QUERY8_RG(0), "region"},
                                              {CHBENCHMARK_QUERY8_NT(0), "nation"},

                                              {CHBENCHMARK_QUERY9_IT(0), "item"},
                                              {CHBENCHMARK_QUERY9_SP(0), "supplier"},
                                              {CHBENCHMARK_QUERY9_ST(0), "stock"},
                                              {CHBENCHMARK_QUERY9_OL(0), "order_line"},
                                              {CHBENCHMARK_QUERY9_OD(0), "order"},
                                              {CHBENCHMARK_QUERY9_NT(0), "nation"},

                                              {CHBENCHMARK_QUERY10_CT(0), "customer"},
                                              {CHBENCHMARK_QUERY10_OL(0), "order_line"},
                                              {CHBENCHMARK_QUERY10_OD(0), "order"},
                                              {CHBENCHMARK_QUERY10_NT(0), "nation"},

                                              {CHBENCHMARK_QUERY11_SP(0), "supplier"},
                                              {CHBENCHMARK_QUERY11_ST(0), "stock"},
                                              {CHBENCHMARK_QUERY11_NT(0), "nation"},

                                              {CHBENCHMARK_QUERY12_OL(0), "order_line"},
                                              {CHBENCHMARK_QUERY12_OD(0), "order"},

                                              {CHBENCHMARK_QUERY13_OD(0), "order"},
                                              {CHBENCHMARK_QUERY13_CT(0), "customer"},

                                              {CHBENCHMARK_QUERY14_OL(0), "order_line"},
                                              {CHBENCHMARK_QUERY14_IT(0), "item"},

                                              {CHBENCHMARK_QUERY15_SP(0), "supplier"},
                                              {CHBENCHMARK_QUERY15_ST(0), "stock"},
                                              {CHBENCHMARK_QUERY15_OL(0), "order_line"},

                                              {CHBENCHMARK_QUERY16_IT(0), "item"},
                                              {CHBENCHMARK_QUERY16_SP(0), "supplier"},
                                              {CHBENCHMARK_QUERY16_ST(0), "stock"},

                                              {CHBENCHMARK_QUERY17_OL(0), "order_line"},
                                              {CHBENCHMARK_QUERY17_IT(0), "item"},

                                              {CHBENCHMARK_QUERY18_CT(0), "customer"},
                                              {CHBENCHMARK_QUERY18_OL(0), "order_line"},
                                              {CHBENCHMARK_QUERY18_OD(0), "order"},

                                              {CHBENCHMARK_QUERY19_OL(0), "order_line"},
                                              {CHBENCHMARK_QUERY19_IT(0), "item"},

                                              {CHBENCHMARK_QUERY20_IT(0), "item"},
                                              {CHBENCHMARK_QUERY20_SP(0), "supplier"},
                                              {CHBENCHMARK_QUERY20_ST(0), "stock"},
                                              {CHBENCHMARK_QUERY20_OL(0), "order_line"},
                                              {CHBENCHMARK_QUERY20_NT(0), "nation"},

                                              {CHBENCHMARK_QUERY21_SP(0), "supplier"},
                                              {CHBENCHMARK_QUERY21_ST(0), "stock"},
                                              {CHBENCHMARK_QUERY21_OL(0), "order_line"},
                                              {CHBENCHMARK_QUERY21_OD(0), "order"},
                                              {CHBENCHMARK_QUERY21_NT(0), "nation"},

                                              {CHBENCHMARK_QUERY22_OD(0), "order"},
                                              {CHBENCHMARK_QUERY22_CT(0), "customer"}};

const map<innid_t, vector<string>> touch_col_map = {
        {CHBENCHMARK_NEW_ORDER_0, {"district.d_next_o_id"}},
        {CHBENCHMARK_NEW_ORDER_1, {}},
        {CHBENCHMARK_NEW_ORDER_2, {}},
        //{CHBENCHMARK_NEW_ORDER_3, {"order.o_d_id", "order.o_w_id", "order.o_id", "order.o_c_id", "order.o_entry_d", "order.o_carrier_id", "order.o_ol_cnt", "order.o_all_local"}},
        //{CHBENCHMARK_NEW_ORDER_4, {"new_order.no_d_id", "new_order.no_w_id", "new_order.no_o_id"}},
        {CHBENCHMARK_NEW_ORDER_RI(0), {}},
        {CHBENCHMARK_NEW_ORDER_RS(0), {}},
        {CHBENCHMARK_NEW_ORDER_WS(0), {"stock.s_quantity", "stock.s_ytd", "stock.s_order_cnt", "stock.s_remote_cnt"}},
        //{CHBENCHMARK_NEW_ORDER_WOL(0), {"order_line.ol_d_id", "order_line.ol_w_id", "order_line.ol_o_id", "order_line.ol_number", "order_line.ol_i_id", "order_line.ol_supply_w_id", "order_line.ol_delivery_d", "order_line.ol_quantity", "order_line.ol_amount", "order_line.ol_dist_info"}},
        {CHBENCHMARK_PAYMENT_0, {}},
        {CHBENCHMARK_PAYMENT_1, {}},
        {CHBENCHMARK_PAYMENT_2, {"district.d_ytd"}},
        {CHBENCHMARK_PAYMENT_3, {}},
        {CHBENCHMARK_PAYMENT_4, {"customer.c_balance", "customer.c_ytd_payment", "customer.c_data"}},
        //{CHBENCHMARK_PAYMENT_5, {"history.h_key", "history.h_c_id", "history.h_c_d_id", "history.h_c_w_id", "history.h_d_id", "history.h_w_id", "history.h_date", "history.h_amount", "history.h_data"}},

        {CHBENCHMARK_ORDER_STATUS_0, {}},
        {CHBENCHMARK_ORDER_STATUS_1, {}},
        {CHBENCHMARK_ORDER_STATUS_2, {}},
        {CHBENCHMARK_ORDER_STATUS_3, {}},

        {CHBENCHMARK_DELIVERY_0, {}},
        {CHBENCHMARK_DELIVERY_1, {"order.o_carrier_id"}},
        {CHBENCHMARK_DELIVERY_2, {"order_line.ol_delivery_d"}},
        {CHBENCHMARK_DELIVERY_3, {"customer.c_balance", "customer.c_delivery_cnt"}},

        {CHBENCHMARK_STOCK_LEVEL_0, {}},
        {CHBENCHMARK_STOCK_LEVEL_1, {}},
        {CHBENCHMARK_STOCK_LEVEL_RS(0), {}},

        {CHBENCHMARK_QUERY1_OL(0), {"order_line.ol_number", "order_line.ol_delivery_d", "order_line.ol_amount", "order_line.ol_quantity"}},

        {CHBENCHMARK_QUERY2_ST(0), {"stock.s_i_id", "stock.s_quantity", "stock.s_su_suppkey"}},
        {CHBENCHMARK_QUERY2_SP(0), {"supplier.su_suppkey", "supplier.su_name", "supplier.su_address", "supplier.su_phone", "supplier.su_comment", "supplier.su_nationkey"}},
        {CHBENCHMARK_QUERY2_IT(0), {"item.i_id", "item.i_name", "item.i_data"}},
        {CHBENCHMARK_QUERY2_NT(0), {"nation.n_nationkey", "nation.n_name", "nation.n_regionkey"}},
        {CHBENCHMARK_QUERY2_RG(0), {"region.r_regionkey", "region.r_name"}},

        {CHBENCHMARK_QUERY3_CT(0), {"customer.c_state", "customer.c_id", "customer.c_w_id", "customer.c_d_id"}},
        {CHBENCHMARK_QUERY3_NO(0), {"new_order.no_w_id", "new_order.no_d_id", "new_order.no_o_id"}},
        {CHBENCHMARK_QUERY3_OD(0), {"order.o_id", "order.o_d_id", "order.o_w_id", "order.o_c_id", "order.o_entry_d"}},
        {CHBENCHMARK_QUERY3_OL(0), {"order_line.ol_o_id", "order_line.ol_d_id", "order_line.ol_w_id", "order_line.ol_amount"}},

        {CHBENCHMARK_QUERY4_OD(0), {"order.o_id", "order.o_d_id", "order.o_w_id", "order.o_entry_d", "order.o_ol_cnt"}},
        {CHBENCHMARK_QUERY4_OL(0), {"order_line.ol_o_id", "order_line.ol_d_id", "order_line.ol_w_id", "order_line.ol_delivery_d"}},

        {CHBENCHMARK_QUERY5_CT(0), {"customer.c_id", "customer.c_d_id", "customer.c_w_id", "customer.c_n_nationkey"}},
        {CHBENCHMARK_QUERY5_OD(0), {"order.o_id", "order.o_d_id", "order.o_w_id", "order.o_c_id", "order.o_entry_d"}},
        {CHBENCHMARK_QUERY5_OL(0), {"order_line.ol_o_id", "order_line.ol_d_id", "order_line.ol_w_id", "order_line.ol_i_id", "order_line.ol_amount"}},
        {CHBENCHMARK_QUERY5_ST(0), {"stock.s_i_id", "stock.s_w_id", "stock.s_su_suppkey"}},
        {CHBENCHMARK_QUERY5_SP(0), {"supplier.su_suppkey", "supplier.su_nationkey"}},
        {CHBENCHMARK_QUERY5_NT(0), {"nation.n_nationkey", "nation.n_name", "nation.n_regionkey"}},
        {CHBENCHMARK_QUERY5_RG(0), {"region.r_regionkey", "region.r_name"}},

        {CHBENCHMARK_QUERY6_OL(0), {"order_line.ol_delivery_d", "order_line.ol_quantity", "order_line.ol_amount"}},

        {CHBENCHMARK_QUERY7_SP(0), {"supplier.su_suppkey", "supplier.su_nationkey"}},
        {CHBENCHMARK_QUERY7_ST(0), {"stock.s_i_id", "stock.s_w_id", "stock.s_su_suppkey"}},
        {CHBENCHMARK_QUERY7_OL(0), {"order_line.ol_o_id", "order_line.ol_d_id", "order_line.ol_w_id", "order_line.ol_i_id", "order_line.ol_supply_w_id", "order_line.ol_delivery_d", "order_line.ol_amount"}},
        {CHBENCHMARK_QUERY7_OD(0), {"order.o_id", "order.o_d_id", "order.o_w_id", "order.o_c_id", "order.o_entry_d"}},
        {CHBENCHMARK_QUERY7_CT(0), {"customer.c_id", "customer.c_d_id", "customer.c_w_id", "customer.c_state", "customer.c_n_nationkey"}},
        {CHBENCHMARK_QUERY7_NT(0), {"nation.n_nationkey", "nation.n_name"}},

        {CHBENCHMARK_QUERY8_IT(0), {"item.i_id", "item.i_data"}},
        {CHBENCHMARK_QUERY8_SP(0), {"supplier.su_suppkey", "supplier.su_nationkey"}},
        {CHBENCHMARK_QUERY8_ST(0), {"stock.s_i_id", "stock.s_w_id", "stock.s_su_suppkey"}},
        {CHBENCHMARK_QUERY8_OL(0), {"order_line.ol_o_id", "order_line.ol_d_id", "order_line.ol_w_id", "order_line.ol_i_id", "order_line.ol_supply_w_id", "order_line.ol_amount"}},
        {CHBENCHMARK_QUERY8_OD(0), {"order.o_id", "order.o_d_id", "order.o_w_id", "order.o_c_id", "order.o_entry_d"}},
        {CHBENCHMARK_QUERY8_CT(0), {"customer.c_id", "customer.c_d_id", "customer.c_w_id", "customer.c_n_nationkey"}},
        {CHBENCHMARK_QUERY8_RG(0), {"region.r_name", "region.r_regionkey"}},
        {CHBENCHMARK_QUERY8_NT(0), {"nation.n_nationkey", "nation.n_name", "nation.n_regionkey"}},

        {CHBENCHMARK_QUERY9_IT(0), {"item.i_id", "item.i_data"}},
        {CHBENCHMARK_QUERY9_SP(0), {"supplier.su_suppkey", "supplier.su_nationkey"}},
        {CHBENCHMARK_QUERY9_ST(0), {"stock.s_i_id", "stock.s_w_id", "stock.s_su_suppkey"}},
        {CHBENCHMARK_QUERY9_OL(0), {"order_line.ol_o_id", "order_line.ol_d_id", "order_line.ol_w_id", "order_line.ol_i_id", "order_line.ol_supply_w_id", "order_line.ol_amount"}},
        {CHBENCHMARK_QUERY9_OD(0), {"order.o_id", "order.o_d_id", "order.o_w_id", "order.o_entry_d"}},
        {CHBENCHMARK_QUERY9_NT(0), {"nation.n_nationkey", "nation.n_name"}},

        {CHBENCHMARK_QUERY10_CT(0), {"customer.c_id", "customer.c_d_id", "customer.c_w_id", "customer.c_last", "customer.c_city", "customer.c_phone", "customer.c_n_nationkey"}},
        {CHBENCHMARK_QUERY10_OL(0), {"order_line.ol_o_id", "order_line.ol_d_id", "order_line.ol_w_id", "order_line.ol_delivery_d", "order_line.ol_amount"}},
        {CHBENCHMARK_QUERY10_OD(0), {"order.o_id", "order.o_d_id", "order.o_w_id", "order.o_c_id", "order.o_entry_d"}},
        {CHBENCHMARK_QUERY10_NT(0), {"nation.n_nationkey", "nation.n_name"}},

        {CHBENCHMARK_QUERY11_SP(0), {"supplier.su_suppkey", "supplier.su_nationkey"}},
        {CHBENCHMARK_QUERY11_ST(0), {"stock.s_i_id", "stock.s_order_cnt", "stock.s_su_suppkey"}},
        {CHBENCHMARK_QUERY11_NT(0), {"nation.n_nationkey", "nation.n_name"}},

        {CHBENCHMARK_QUERY12_OL(0), {"order_line.ol_o_id", "order_line.ol_d_id", "order_line.ol_w_id", "order_line.ol_delivery_d"}},
        {CHBENCHMARK_QUERY12_OD(0), {"order.o_id", "order.o_d_id", "order.o_w_id", "order.o_entry_d", "order.o_ol_cnt", "order.o_carrier_id"}},

        {CHBENCHMARK_QUERY13_OD(0), {"order.o_id", "order.o_d_id", "order.o_w_id", "order.o_c_id", "order.o_carrier_id"}},
        {CHBENCHMARK_QUERY13_CT(0), {"customer.c_id", "customer.c_d_id", "customer.c_w_id"}},

        {CHBENCHMARK_QUERY14_OL(0), {"order_line.ol_i_id", "order_line.ol_delivery_d", "order_line.ol_amount"}},
        {CHBENCHMARK_QUERY14_IT(0), {"item.i_id", "item.i_data"}},

        {CHBENCHMARK_QUERY15_SP(0), {"supplier.su_suppkey", "supplier.su_name", "supplier.su_address", "supplier.su_phone"}},
        {CHBENCHMARK_QUERY15_ST(0), {"stock.s_i_id", "stock.s_w_id", "stock.s_su_suppkey"}},
        {CHBENCHMARK_QUERY15_OL(0), {"order_line.ol_i_id", "order_line.ol_supply_w_id", "order_line.ol_delivery_d", "order_line.ol_amount"}},

        {CHBENCHMARK_QUERY16_IT(0), {"item.i_id", "item.i_data", "item.i_name", "item.i_price"}},
        {CHBENCHMARK_QUERY16_SP(0), {"supplier.su_suppkey", "supplier.su_comment"}},
        {CHBENCHMARK_QUERY16_ST(0), {"stock.s_i_id", "stock.s_su_suppkey"}},

        {CHBENCHMARK_QUERY17_OL(0), {"order_line.ol_i_id", "order_line.ol_quantity", "order_line.ol_amount"}},
        {CHBENCHMARK_QUERY17_IT(0), {"item.i_id", "item.i_data"}},

        {CHBENCHMARK_QUERY18_CT(0), {"customer.c_id", "customer.c_d_id", "customer.c_w_id", "customer.c_last"}},
        {CHBENCHMARK_QUERY18_OL(0), {"order_line.ol_o_id", "order_line.ol_d_id", "order_line.ol_w_id", "order_line.ol_amount"}},
        {CHBENCHMARK_QUERY18_OD(0), {"order.o_id", "order.o_d_id", "order.o_w_id", "order.o_c_id", "order.o_entry_d", "order.o_ol_cnt"}},

        {CHBENCHMARK_QUERY19_OL(0), {"order_line.ol_i_id", "order_line.ol_w_id", "order_line.ol_quantity", "order_line.ol_amount"}},
        {CHBENCHMARK_QUERY19_IT(0), {"item.i_id", "item.i_data", "item.i_price"}},

        {CHBENCHMARK_QUERY20_IT(0), {"item.i_id", "item.i_data"}},
        {CHBENCHMARK_QUERY20_SP(0), {"supplier.su_suppkey", "supplier.su_nationkey", "supplier.su_name", "supplier.su_address"}},
        {CHBENCHMARK_QUERY20_ST(0), {"stock.s_i_id", "stock.s_w_id", "stock.s_quantity"}},
        {CHBENCHMARK_QUERY20_OL(0), {"order_line.ol_i_id", "order_line.ol_delivery_d", "order_line.ol_quantity"}},
        {CHBENCHMARK_QUERY20_NT(0), {"nation.n_nationkey", "nation.n_name"}},

        {CHBENCHMARK_QUERY21_SP(0), {"supplier.su_suppkey", "supplier.su_nationkey", "supplier.su_name"}},
        {CHBENCHMARK_QUERY21_ST(0), {"stock.s_i_id", "stock.s_w_id", "stock.s_su_suppkey"}},
        {CHBENCHMARK_QUERY21_OL(0), {"order_line.ol_o_id", "order_line.ol_d_id", "order_line.ol_w_id", "order_line.ol_i_id", "order_line.ol_delivery_d"}},
        {CHBENCHMARK_QUERY21_OD(0), {"order.o_id", "order.o_d_id", "order.o_w_id", "order.o_entry_d"}},
        {CHBENCHMARK_QUERY21_NT(0), {"nation.n_nationkey", "nation.n_name"}},

        {CHBENCHMARK_QUERY22_OD(0), {"order.o_d_id", "order.o_w_id", "order.o_c_id", "order.o_carrier_id"}},
        {CHBENCHMARK_QUERY22_CT(0), {"customer.c_id", "customer.c_d_id", "customer.c_w_id", "customer.c_state", "customer.c_phone", "customer.c_balance"}},
};
class RccGraph;
class BrqCommo;
class BrqSched : public RccSched {
public:
    using RccSched::RccSched;

    map<txnid_t, RccDTxn *> Aggregate(RccGraph &graph);

    void OnPreAccept(const txnid_t txnid,
                     const vector<SimpleCommand> &cmds,
                     const RccGraph &graph,
                     int32_t *res,
                     RccGraph *res_graph,
                     function<void()> callback);

    void OnPreAcceptWoGraph(const txnid_t txnid,
                            const vector<SimpleCommand> &cmds,
                            int32_t *res,
                            RccGraph *res_graph,
                            function<void()> callback);

    void OnAccept(const txnid_t txn_id,
                  const ballot_t &ballot,
                  const RccGraph &graph,
                  int32_t *res,
                  function<void()> callback);

    void OnSendOutput(const ChronosSendOutputReq &chr_req,
                      ChronosSendOutputRes *chr_res);
    //  void OnCommit(const txnid_t txn_id,
    //                const RccGraph &graph,
    //                int32_t *res,
    //                TxnOutput *output,
    //                const function<void()> &callback);


    void OnCommit(const txnid_t txn_id,
                  RccGraph *graph,
                  int32_t fast_path_,
                  int32_t *res,
                  TxnOutput *output,
                  const function<void()> &callback);

    //  void OnCommitWoGraph(const txnid_t cmd_id,
    //                       int32_t* res,
    //                       TxnOutput* output,
    //                       const function<void()>& callback);

    int OnInquire(epoch_t epoch,
                  cmdid_t cmd_id,
                  RccGraph *graph,
                  const function<void()> &callback) override;
    BrqCommo *commo();
};
}// namespace rococo