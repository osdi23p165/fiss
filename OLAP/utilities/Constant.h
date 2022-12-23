#include "_dep_.h"

typedef struct TPInfo
{
    vector<int> ShardList;
    vector<string> ColIDList;
    map<string, int> ColIDIndex;
} TPInfo;

// ======== Query1 txn =========
#define CHBENCHMARK_QUERY1 61
#define CHBENCHMARK_QUERY1_NAME "QUERY1"
#define CHBENCHMARK_QUERY1_OL(i) (611000 + i)

// ======== Query2 txn =========
#define CHBENCHMARK_QUERY2 62
#define CHBENCHMARK_QUERY2_NAME "QUERY2"
#define CHBENCHMARK_QUERY2_ST(i) (621000 + i)
#define CHBENCHMARK_QUERY2_SP(i) (622000 + i)
#define CHBENCHMARK_QUERY2_IT(i) (623000 + i)
#define CHBENCHMARK_QUERY2_NT(i) (624000 + i)
#define CHBENCHMARK_QUERY2_RG(i) (625000 + i)

// ======== Query3 txn =========
#define CHBENCHMARK_QUERY3 63
#define CHBENCHMARK_QUERY3_NAME "QUERY3"
#define CHBENCHMARK_QUERY3_CT(i) (631000 + i)
#define CHBENCHMARK_QUERY3_NO(i) (632000 + i)
#define CHBENCHMARK_QUERY3_OD(i) (633000 + i)
#define CHBENCHMARK_QUERY3_OL(i) (634000 + i)

// ======== Query4 txn =========
#define CHBENCHMARK_QUERY4 64
#define CHBENCHMARK_QUERY4_NAME "QUERY4"
#define CHBENCHMARK_QUERY4_OD(i) (641000 + i)
#define CHBENCHMARK_QUERY4_OL(i) (642000 + i)

// ======== Query5 txn =========
#define CHBENCHMARK_QUERY5 65
#define CHBENCHMARK_QUERY5_NAME "QUERY5"
#define CHBENCHMARK_QUERY5_CT(i) (651000 + i)
#define CHBENCHMARK_QUERY5_OD(i) (652000 + i)
#define CHBENCHMARK_QUERY5_OL(i) (653000 + i)
#define CHBENCHMARK_QUERY5_ST(i) (654000 + i)
#define CHBENCHMARK_QUERY5_SP(i) (655000 + i)
#define CHBENCHMARK_QUERY5_NT(i) (656000 + i)
#define CHBENCHMARK_QUERY5_RG(i) (657000 + i)

// ======== Query6 txn =========
#define CHBENCHMARK_QUERY6 66
#define CHBENCHMARK_QUERY6_NAME "QUERY6"
#define CHBENCHMARK_QUERY6_OL(i) (661000 + i)

// ======== Query7 txn =========
#define CHBENCHMARK_QUERY7 67
#define CHBENCHMARK_QUERY7_NAME "QUERY7"
#define CHBENCHMARK_QUERY7_SP(i) (671000 + i)
#define CHBENCHMARK_QUERY7_ST(i) (672000 + i)
#define CHBENCHMARK_QUERY7_OL(i) (673000 + i)
#define CHBENCHMARK_QUERY7_OD(i) (674000 + i)
#define CHBENCHMARK_QUERY7_CT(i) (675000 + i)
#define CHBENCHMARK_QUERY7_NT(i) (676000 + i)

// ======== Query8 txn =========
#define CHBENCHMARK_QUERY8 68
#define CHBENCHMARK_QUERY8_NAME "QUERY8"
#define CHBENCHMARK_QUERY8_IT(i) (681000 + i)
#define CHBENCHMARK_QUERY8_SP(i) (682000 + i)
#define CHBENCHMARK_QUERY8_ST(i) (683000 + i)
#define CHBENCHMARK_QUERY8_OL(i) (684000 + i)
#define CHBENCHMARK_QUERY8_OD(i) (685000 + i)
#define CHBENCHMARK_QUERY8_CT(i) (686000 + i)
#define CHBENCHMARK_QUERY8_RG(i) (687000 + i)
#define CHBENCHMARK_QUERY8_NT(i) (688000 + i)

// ======== Query9 txn =========
#define CHBENCHMARK_QUERY9 69
#define CHBENCHMARK_QUERY9_NAME "QUERY9"
#define CHBENCHMARK_QUERY9_IT(i) (691000 + i)
#define CHBENCHMARK_QUERY9_SP(i) (692000 + i)
#define CHBENCHMARK_QUERY9_ST(i) (693000 + i)
#define CHBENCHMARK_QUERY9_OL(i) (694000 + i)
#define CHBENCHMARK_QUERY9_OD(i) (695000 + i)
#define CHBENCHMARK_QUERY9_NT(i) (696000 + i)

// ======== Query10 txn =========
#define CHBENCHMARK_QUERY10 70
#define CHBENCHMARK_QUERY10_NAME "QUERY10"
#define CHBENCHMARK_QUERY10_CT(i) (701000 + i)
#define CHBENCHMARK_QUERY10_OD(i) (702000 + i)
#define CHBENCHMARK_QUERY10_OL(i) (703000 + i)
#define CHBENCHMARK_QUERY10_NT(i) (704000 + i)

// ======== Query11 txn =========
#define CHBENCHMARK_QUERY11 71
#define CHBENCHMARK_QUERY11_NAME "QUERY11"
#define CHBENCHMARK_QUERY11_ST(i) (711000 + i)
#define CHBENCHMARK_QUERY11_SP(i) (712000 + i)
#define CHBENCHMARK_QUERY11_NT(i) (713000 + i)

// ======== Query12 txn =========
#define CHBENCHMARK_QUERY12 72
#define CHBENCHMARK_QUERY12_NAME "QUERY12"
#define CHBENCHMARK_QUERY12_OD(i) (721000 + i)
#define CHBENCHMARK_QUERY12_OL(i) (722000 + i)

// ======== Query13 txn =========
#define CHBENCHMARK_QUERY13 73
#define CHBENCHMARK_QUERY13_NAME "QUERY13"
#define CHBENCHMARK_QUERY13_CT(i) (731000 + i)
#define CHBENCHMARK_QUERY13_OD(i) (732000 + i)

// ======== Query14 txn =========
#define CHBENCHMARK_QUERY14 74
#define CHBENCHMARK_QUERY14_NAME "QUERY14"
#define CHBENCHMARK_QUERY14_OL(i) (741000 + i)
#define CHBENCHMARK_QUERY14_IT(i) (742000 + i)

// ======== Query15 txn =========
#define CHBENCHMARK_QUERY15 75
#define CHBENCHMARK_QUERY15_NAME "QUERY15"
#define CHBENCHMARK_QUERY15_OL(i) (751000 + i)
#define CHBENCHMARK_QUERY15_ST(i) (752000 + i)
#define CHBENCHMARK_QUERY15_SP(i) (753000 + i)

// ======== Query16 txn =========
#define CHBENCHMARK_QUERY16 76
#define CHBENCHMARK_QUERY16_NAME "QUERY16"
#define CHBENCHMARK_QUERY16_ST(i) (761000 + i)
#define CHBENCHMARK_QUERY16_IT(i) (762000 + i)
#define CHBENCHMARK_QUERY16_SP(i) (763000 + i)

// ======== Query17 txn =========
#define CHBENCHMARK_QUERY17 77
#define CHBENCHMARK_QUERY17_NAME "QUERY17"
#define CHBENCHMARK_QUERY17_IT(i) (771000 + i)
#define CHBENCHMARK_QUERY17_OL(i) (772000 + i)

// ======== Query18 txn =========
#define CHBENCHMARK_QUERY18 78
#define CHBENCHMARK_QUERY18_NAME "QUERY18"
#define CHBENCHMARK_QUERY18_CT(i) (781000 + i)
#define CHBENCHMARK_QUERY18_OD(i) (782000 + i)
#define CHBENCHMARK_QUERY18_OL(i) (783000 + i)

// ======== Query19 txn =========
#define CHBENCHMARK_QUERY19 79
#define CHBENCHMARK_QUERY19_NAME "QUERY19"
#define CHBENCHMARK_QUERY19_OL(i) (791000 + i)
#define CHBENCHMARK_QUERY19_IT(i) (792000 + i)

// ======== Query20 txn =========
#define CHBENCHMARK_QUERY20 80
#define CHBENCHMARK_QUERY20_NAME "QUERY20"
#define CHBENCHMARK_QUERY20_SP(i) (801000 + i)
#define CHBENCHMARK_QUERY20_NT(i) (802000 + i)
#define CHBENCHMARK_QUERY20_ST(i) (803000 + i)
#define CHBENCHMARK_QUERY20_OL(i) (804000 + i)
#define CHBENCHMARK_QUERY20_IT(i) (805000 + i)

// ======== Query21 txn =========
#define CHBENCHMARK_QUERY21 81
#define CHBENCHMARK_QUERY21_NAME "QUERY21"
#define CHBENCHMARK_QUERY21_SP(i) (811000 + i)
#define CHBENCHMARK_QUERY21_OL(i) (812000 + i)
#define CHBENCHMARK_QUERY21_OD(i) (813000 + i)
#define CHBENCHMARK_QUERY21_ST(i) (814000 + i)
#define CHBENCHMARK_QUERY21_NT(i) (815000 + i)

// ======== Query22 txn =========
#define CHBENCHMARK_QUERY22 82
#define CHBENCHMARK_QUERY22_NAME "QUERY22"
#define CHBENCHMARK_QUERY22_CT(i) (821000 + i)
#define CHBENCHMARK_QUERY22_OD(i) (822000 + i)

static map<int, vector<string>> query_touch_col_map = {
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

static TPInfo tp_info_ = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29},
    {"district.d_next_o_id","district.d_ytd","order.o_d_id","order.o_w_id","order.o_id",
    "order.o_c_id","order.o_entry_d","order.o_carrier_id","order.o_ol_cnt","order.o_all_local",
    "new_order.no_d_id","new_order.no_w_id","new_order.no_o_id","stock.s_quantity","stock.s_ytd",
    "stock.s_order_cnt","stock.s_remote_cnt","order_line.ol_d_id","order_line.ol_w_id","order_line.ol_o_id",
    "order_line.ol_number","order_line.ol_i_id","order_line.ol_supply_w_id","order_line.ol_delivery_d",
    "order_line.ol_quantity","order_line.ol_amount","order_line.ol_dist_info","customer.c_balance",
    "customer.c_ytd_payment","customer.c_data","customer.c_delivery_cnt","history.h_key",
    "history.h_c_id","history.h_c_d_id","history.h_c_w_id","history.h_d_id","history.h_w_id",
    "history.h_date","history.h_amount","history.h_data"},
    {{"district.d_next_o_id", 0}, {"district.d_ytd", 1}, {"order.o_d_id", 2}, {"order.o_w_id", 3}, {"order.o_id", 4}, 
    {"order.o_c_id", 5}, {"order.o_entry_d", 6}, {"order.o_carrier_id", 7}, {"order.o_ol_cnt", 8}, {"order.o_all_local", 9}, 
    {"new_order.no_d_id", 10}, {"new_order.no_w_id", 11}, {"new_order.no_o_id", 12}, {"stock.s_quantity", 13}, {"stock.s_ytd", 14}, 
    {"stock.s_order_cnt", 15}, {"stock.s_remote_cnt", 16}, {"order_line.ol_d_id", 17}, {"order_line.ol_w_id", 18}, {"order_line.ol_o_id", 19}, 
    {"order_line.ol_number", 20}, {"order_line.ol_i_id", 21}, {"order_line.ol_supply_w_id", 22}, {"order_line.ol_delivery_d", 23}, 
    {"order_line.ol_quantity", 24}, {"order_line.ol_amount", 25}, {"order_line.ol_dist_info", 26}, {"customer.c_balance", 27}, 
    {"customer.c_ytd_payment", 28}, {"customer.c_data", 29}, {"customer.c_delivery_cnt", 30}, {"history.h_key", 31}, 
    {"history.h_c_id", 32}, {"history.h_c_d_id", 33}, {"history.h_c_w_id", 34}, {"history.h_d_id", 35}, {"history.h_w_id", 36}, 
    {"history.h_date", 37}, {"history.h_amount", 38}, {"history.h_data", 39}}
};