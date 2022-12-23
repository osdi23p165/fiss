#ifndef CHBENCHMARK_PIECE_H_
#define CHBENCHMARK_PIECE_H_

#include "deptran/piece.h"

namespace rococo {


// ======= New order txn =======
#define CHBENCHMARK_NEW_ORDER              10
#define CHBENCHMARK_NEW_ORDER_NAME         "NEW_ORDER"
#define CHBENCHMARK_NEW_ORDER_0            1000
#define CHBENCHMARK_NEW_ORDER_1            1001
#define CHBENCHMARK_NEW_ORDER_2            1002
#define CHBENCHMARK_NEW_ORDER_3            1003
#define CHBENCHMARK_NEW_ORDER_4            1004
//#define CHBENCHMARK_NEW_ORDER_5            105
#define CHBENCHMARK_NEW_ORDER_RI(i)           (15000+i)
#define CHBENCHMARK_NEW_ORDER_RS(i)           (16000+i)
#define CHBENCHMARK_NEW_ORDER_WS(i)           (17000+i)
#define CHBENCHMARK_NEW_ORDER_WOL(i)          (18000+i)

// ======== Payment txn ========
#define CHBENCHMARK_PAYMENT                20
#define CHBENCHMARK_PAYMENT_NAME           "PAYMENT"
#define CHBENCHMARK_PAYMENT_0              200
#define CHBENCHMARK_PAYMENT_1              201
#define CHBENCHMARK_PAYMENT_2              202
#define CHBENCHMARK_PAYMENT_3              203
#define CHBENCHMARK_PAYMENT_4              204
#define CHBENCHMARK_PAYMENT_5              205

// ===== Order status txn ======
#define CHBENCHMARK_ORDER_STATUS           30
#define CHBENCHMARK_ORDER_STATUS_NAME      "ORDER_STATUS"
#define CHBENCHMARK_ORDER_STATUS_0         300
#define CHBENCHMARK_ORDER_STATUS_1         301
#define CHBENCHMARK_ORDER_STATUS_2         302
#define CHBENCHMARK_ORDER_STATUS_3         303

// ======= Delivery txn ========
#define CHBENCHMARK_DELIVERY               40
#define CHBENCHMARK_DELIVERY_NAME          "DELIVERY"
#define CHBENCHMARK_DELIVERY_0             400
#define CHBENCHMARK_DELIVERY_1             401
#define CHBENCHMARK_DELIVERY_2             402
#define CHBENCHMARK_DELIVERY_3             403

// ====== Stock level txn ======
#define CHBENCHMARK_STOCK_LEVEL            50
#define CHBENCHMARK_STOCK_LEVEL_NAME       "STOCK_LEVEL"
#define CHBENCHMARK_STOCK_LEVEL_0          500
#define CHBENCHMARK_STOCK_LEVEL_1          501
#define CHBENCHMARK_STOCK_LEVEL_RS(i)          (52000+i)

// ======== Query1 txn =========
#define CHBENCHMARK_QUERY1 61
#define CHBENCHMARK_QUERY1_NAME "QUERY1"
#define CHBENCHMARK_QUERY1_OL(i) (611000+i)

// ======== Query2 txn =========
#define CHBENCHMARK_QUERY2 62
#define CHBENCHMARK_QUERY2_NAME "QUERY2"
#define CHBENCHMARK_QUERY2_ST(i) (621000+i)
#define CHBENCHMARK_QUERY2_SP(i) (622000+i)
#define CHBENCHMARK_QUERY2_IT(i) (623000+i)
#define CHBENCHMARK_QUERY2_NT(i) (624000+i)
#define CHBENCHMARK_QUERY2_RG(i) (625000+i)

// ======== Query3 txn =========
#define CHBENCHMARK_QUERY3 63
#define CHBENCHMARK_QUERY3_NAME "QUERY3"
#define CHBENCHMARK_QUERY3_CT(i) (631000+i)
#define CHBENCHMARK_QUERY3_NO(i) (632000+i)
#define CHBENCHMARK_QUERY3_OD(i) (633000+i)
#define CHBENCHMARK_QUERY3_OL(i) (634000+i)

// ======== Query4 txn =========
#define CHBENCHMARK_QUERY4 64
#define CHBENCHMARK_QUERY4_NAME "QUERY4"
#define CHBENCHMARK_QUERY4_OD(i) (641000+i)
#define CHBENCHMARK_QUERY4_OL(i) (642000+i)

// ======== Query5 txn =========
#define CHBENCHMARK_QUERY5 65
#define CHBENCHMARK_QUERY5_NAME "QUERY5"
#define CHBENCHMARK_QUERY5_CT(i) (651000+i)
#define CHBENCHMARK_QUERY5_OD(i) (652000+i)
#define CHBENCHMARK_QUERY5_OL(i) (653000+i)
#define CHBENCHMARK_QUERY5_ST(i) (654000+i)
#define CHBENCHMARK_QUERY5_SP(i) (655000+i)
#define CHBENCHMARK_QUERY5_NT(i) (656000+i)
#define CHBENCHMARK_QUERY5_RG(i) (657000+i)

// ======== Query6 txn =========
#define CHBENCHMARK_QUERY6 66
#define CHBENCHMARK_QUERY6_NAME "QUERY6"
#define CHBENCHMARK_QUERY6_OL(i) (661000+i)

// ======== Query7 txn =========
#define CHBENCHMARK_QUERY7 67
#define CHBENCHMARK_QUERY7_NAME "QUERY7"
#define CHBENCHMARK_QUERY7_SP(i) (671000+i)
#define CHBENCHMARK_QUERY7_ST(i) (672000+i)
#define CHBENCHMARK_QUERY7_OL(i) (673000+i)
#define CHBENCHMARK_QUERY7_OD(i) (674000+i)
#define CHBENCHMARK_QUERY7_CT(i) (675000+i)
#define CHBENCHMARK_QUERY7_NT(i) (676000+i)

// ======== Query8 txn =========
#define CHBENCHMARK_QUERY8 68
#define CHBENCHMARK_QUERY8_NAME "QUERY8"
#define CHBENCHMARK_QUERY8_IT(i) (681000+i)
#define CHBENCHMARK_QUERY8_SP(i) (682000+i)
#define CHBENCHMARK_QUERY8_ST(i) (683000+i)
#define CHBENCHMARK_QUERY8_OL(i) (684000+i)
#define CHBENCHMARK_QUERY8_OD(i) (685000+i)
#define CHBENCHMARK_QUERY8_CT(i) (686000+i)
#define CHBENCHMARK_QUERY8_RG(i) (687000+i)
#define CHBENCHMARK_QUERY8_NT(i) (688000+i)

// ======== Query9 txn =========
#define CHBENCHMARK_QUERY9 69
#define CHBENCHMARK_QUERY9_NAME "QUERY9"
#define CHBENCHMARK_QUERY9_IT(i) (691000+i)
#define CHBENCHMARK_QUERY9_SP(i) (692000+i)
#define CHBENCHMARK_QUERY9_ST(i) (693000+i)
#define CHBENCHMARK_QUERY9_OL(i) (694000+i)
#define CHBENCHMARK_QUERY9_OD(i) (695000+i)
#define CHBENCHMARK_QUERY9_NT(i) (696000+i)

// ======== Query10 txn =========
#define CHBENCHMARK_QUERY10 70
#define CHBENCHMARK_QUERY10_NAME "QUERY10"
#define CHBENCHMARK_QUERY10_CT(i) (701000+i)
#define CHBENCHMARK_QUERY10_OD(i) (702000+i)
#define CHBENCHMARK_QUERY10_OL(i) (703000+i)
#define CHBENCHMARK_QUERY10_NT(i) (704000+i)

// ======== Query11 txn =========
#define CHBENCHMARK_QUERY11 71
#define CHBENCHMARK_QUERY11_NAME "QUERY11"
#define CHBENCHMARK_QUERY11_ST(i) (711000+i)
#define CHBENCHMARK_QUERY11_SP(i) (712000+i)
#define CHBENCHMARK_QUERY11_NT(i) (713000+i)

// ======== Query12 txn =========
#define CHBENCHMARK_QUERY12 72
#define CHBENCHMARK_QUERY12_NAME "QUERY12"
#define CHBENCHMARK_QUERY12_OD(i) (721000+i)
#define CHBENCHMARK_QUERY12_OL(i) (722000+i)

// ======== Query13 txn =========
#define CHBENCHMARK_QUERY13 73
#define CHBENCHMARK_QUERY13_NAME "QUERY13"
#define CHBENCHMARK_QUERY13_CT(i) (731000+i)
#define CHBENCHMARK_QUERY13_OD(i) (732000+i)

// ======== Query14 txn =========
#define CHBENCHMARK_QUERY14 74
#define CHBENCHMARK_QUERY14_NAME "QUERY14"
#define CHBENCHMARK_QUERY14_OL(i) (741000+i)
#define CHBENCHMARK_QUERY14_IT(i) (742000+i)

// ======== Query15 txn =========
#define CHBENCHMARK_QUERY15 75
#define CHBENCHMARK_QUERY15_NAME "QUERY15"
#define CHBENCHMARK_QUERY15_OL(i) (751000+i)
#define CHBENCHMARK_QUERY15_ST(i) (752000+i)
#define CHBENCHMARK_QUERY15_SP(i) (753000+i)

// ======== Query16 txn =========
#define CHBENCHMARK_QUERY16 76
#define CHBENCHMARK_QUERY16_NAME "QUERY16"
#define CHBENCHMARK_QUERY16_ST(i) (761000+i)
#define CHBENCHMARK_QUERY16_IT(i) (762000+i)
#define CHBENCHMARK_QUERY16_SP(i) (763000+i)

// ======== Query17 txn =========
#define CHBENCHMARK_QUERY17 77
#define CHBENCHMARK_QUERY17_NAME "QUERY17"
#define CHBENCHMARK_QUERY17_IT(i) (771000+i)
#define CHBENCHMARK_QUERY17_OL(i) (772000+i)

// ======== Query18 txn =========
#define CHBENCHMARK_QUERY18 78
#define CHBENCHMARK_QUERY18_NAME "QUERY18"
#define CHBENCHMARK_QUERY18_CT(i) (781000+i)
#define CHBENCHMARK_QUERY18_OD(i) (782000+i)
#define CHBENCHMARK_QUERY18_OL(i) (783000+i)

// ======== Query19 txn =========
#define CHBENCHMARK_QUERY19 79
#define CHBENCHMARK_QUERY19_NAME "QUERY19"
#define CHBENCHMARK_QUERY19_OL(i) (791000+i)
#define CHBENCHMARK_QUERY19_IT(i) (792000+i)

// ======== Query20 txn =========
#define CHBENCHMARK_QUERY20 80
#define CHBENCHMARK_QUERY20_NAME "QUERY20"
#define CHBENCHMARK_QUERY20_SP(i) (801000+i)
#define CHBENCHMARK_QUERY20_NT(i) (802000+i)
#define CHBENCHMARK_QUERY20_ST(i) (803000+i)
#define CHBENCHMARK_QUERY20_OL(i) (804000+i)
#define CHBENCHMARK_QUERY20_IT(i) (805000+i)

// ======== Query21 txn =========
#define CHBENCHMARK_QUERY21 81
#define CHBENCHMARK_QUERY21_NAME "QUERY21"
#define CHBENCHMARK_QUERY21_SP(i) (811000+i)
#define CHBENCHMARK_QUERY21_OL(i) (812000+i)
#define CHBENCHMARK_QUERY21_OD(i) (813000+i)
#define CHBENCHMARK_QUERY21_ST(i) (814000+i)
#define CHBENCHMARK_QUERY21_NT(i) (815000+i)

// ======== Query22 txn =========
#define CHBENCHMARK_QUERY22 82
#define CHBENCHMARK_QUERY22_NAME "QUERY22"
#define CHBENCHMARK_QUERY22_CT(i) (821000+i)
#define CHBENCHMARK_QUERY22_OD(i) (822000+i)

// context id
#define ROW_DISTRICT          __LINE__
#define ROW_DISTRICT_TEMP     __LINE__
#define ROW_WAREHOUSE         __LINE__
#define ROW_CUSTOMER          __LINE__
#define ROW_ORDER             __LINE__
#define RS_NEW_ORDER          __LINE__
#define RS_ORDER_LINE         __LINE__
#define ROW_ORDER_SEC         (cmd.id_)
#define ROW_ITEM              (cmd.id_)
#define ROW_STOCK             (cmd.id_)
#define ROW_STOCK_TEMP        (cmd.id_)
#define ROW_NATION            (cmd.id_)
#define ROW_SUPPLIER          (cmd.id_)

#define CHBENCHMARK_COL_WAREHOUSE_W_NAME              (1)
#define CHBENCHMARK_COL_WAREHOUSE_W_STREET_1          (2)
#define CHBENCHMARK_COL_WAREHOUSE_W_STREET_2          (3)
#define CHBENCHMARK_COL_WAREHOUSE_W_CITY              (4)
#define CHBENCHMARK_COL_WAREHOUSE_W_STATE             (5)
#define CHBENCHMARK_COL_WAREHOUSE_W_ZIP               (6)
#define CHBENCHMARK_COL_WAREHOUSE_W_TAX               (7)

#define CHBENCHMARK_COL_NEW_ORDER_NO_O_ID             (0)
#define CHBENCHMARK_COL_NEW_ORDER_NO_D_ID             (1)
#define CHBENCHMARK_COL_NEW_ORDER_NO_W_ID             (2)


#define CHBENCHMARK_COL_DISTRICT_D_NAME               (2)
#define CHBENCHMARK_COL_DISTRICT_D_STREET_1           (3)
#define CHBENCHMARK_COL_DISTRICT_D_STREET_2           (4)
#define CHBENCHMARK_COL_DISTRICT_D_CITY               (5)
#define CHBENCHMARK_COL_DISTRICT_D_STATE              (6)
#define CHBENCHMARK_COL_DISTRICT_D_ZIP                (7)
#define CHBENCHMARK_COL_DISTRICT_D_TAX                (8)
#define CHBENCHMARK_COL_DISTRICT_D_YTD                (9)
#define CHBENCHMARK_COL_DISTRICT_D_NEXT_O_ID          (10)

#define CHBENCHMARK_COL_HISTORY_H_KEY                (0)
#define CHBENCHMARK_COL_HISTORY_H_C_ID               (1)
#define CHBENCHMARK_COL_HISTORY_H_C_D_ID             (2)
#define CHBENCHMARK_COL_HISTORY_H_C_W_ID             (3)
#define CHBENCHMARK_COL_HISTORY_H_D_ID               (4)
#define CHBENCHMARK_COL_HISTORY_H_W_ID               (5)
#define CHBENCHMARK_COL_HISTORY_H_DATE               (6)
#define CHBENCHMARK_COL_HISTORY_H_AMOUNT             (7)
#define CHBENCHMARK_COL_HISTORY_H_DATA               (8)

#define CHBENCHMARK_COL_CUSTOMER_C_FIRST              (3)  /* c_first      */
#define CHBENCHMARK_COL_CUSTOMER_C_MIDDLE             (4)  /* c_middle     */
#define CHBENCHMARK_COL_CUSTOMER_C_LAST               (5)  /* c_last       */
#define CHBENCHMARK_COL_CUSTOMER_C_STREET_1           (6)  /* c_street_1   */
#define CHBENCHMARK_COL_CUSTOMER_C_STREET_2           (7)  /* c_street_2   */
#define CHBENCHMARK_COL_CUSTOMER_C_CITY               (8)  /* c_city       */
#define CHBENCHMARK_COL_CUSTOMER_C_STATE              (9)  /* c_state      */
#define CHBENCHMARK_COL_CUSTOMER_C_ZIP                (10) /* c_zip        */
#define CHBENCHMARK_COL_CUSTOMER_C_PHONE              (11) /* c_phone      */
#define CHBENCHMARK_COL_CUSTOMER_C_SINCE              (12) /* c_since      */
#define CHBENCHMARK_COL_CUSTOMER_C_CREDIT             (13) /* c_credit     */
#define CHBENCHMARK_COL_CUSTOMER_C_CREDIT_LIM         (14) /* c_credit_lim */
#define CHBENCHMARK_COL_CUSTOMER_C_DISCOUNT           (15) /* c_discount   */
#define CHBENCHMARK_COL_CUSTOMER_C_BALANCE            (16) /* c_balance    */
#define CHBENCHMARK_COL_CUSTOMER_C_YTD_PAYMENT        (17) /* c_ytd_payment*/
#define CHBENCHMARK_COL_CUSTOMER_C_PAYMENT_CNT        (18) /* c_ytd_payment*/
#define CHBENCHMARK_COL_CUSTOMER_C_DELIVERY_CNT       (19) /* c_ytd_payment*/
#define CHBENCHMARK_COL_CUSTOMER_C_DATA               (20) /* c_data       */

#define CHBENCHMARK_COL_STOCK_S_I_ID                  (0)
#define CHBENCHMARK_COL_STOCK_S_W_ID                  (1)
#define CHBENCHMARK_COL_STOCK_S_QUANTITY              (2)
#define CHBENCHMARK_COL_STOCK_S_DIST_XX               (3)
#define CHBENCHMARK_COL_STOCK_S_YTD                   (13)
#define CHBENCHMARK_COL_STOCK_S_ORDER_CNT             (14)
#define CHBENCHMARK_COL_STOCK_S_REMOTE_CNT            (15)
#define CHBENCHMARK_COL_STOCK_S_DATA                  (16)

#define CHBENCHMARK_COL_ORDER_O_ID                    (0)
#define CHBENCHMARK_COL_ORDER_O_D_ID                  (1)
#define CHBENCHMARK_COL_ORDER_O_W_ID                  (2)
#define CHBENCHMARK_COL_ORDER_O_C_ID                  (3)
#define CHBENCHMARK_COL_ORDER_O_ENTRY_D               (4)
#define CHBENCHMARK_COL_ORDER_O_CARRIER_ID            (5)
#define CHBENCHMARK_COL_ORDER_O_OL_CNT                (6)
#define CHBENCHMARK_COL_ORDER_O_ALL_LOCAL             (7)

#define CHBENCHMARK_COL_ORDER_LINE_OL_O_ID            (0)
#define CHBENCHMARK_COL_ORDER_LINE_OL_D_ID            (1)
#define CHBENCHMARK_COL_ORDER_LINE_OL_W_ID            (2)
#define CHBENCHMARK_COL_ORDER_LINE_OL_NUMBER          (3)
#define CHBENCHMARK_COL_ORDER_LINE_OL_I_ID            (4)
#define CHBENCHMARK_COL_ORDER_LINE_OL_SUPPLY_W_ID     (5)
#define CHBENCHMARK_COL_ORDER_LINE_OL_DELIVERY_D      (6)
#define CHBENCHMARK_COL_ORDER_LINE_OL_QUANTITY        (7)
#define CHBENCHMARK_COL_ORDER_LINE_OL_AMOUNT          (8)
#define CHBENCHMARK_COL_ORDER_LINE_OL_DIST_INFO       (9)

#define CHBENCHMARK_COL_ITEM_I_ID                     (0)
#define CHBENCHMARK_COL_ITEM_I_IM_ID                  (1)
#define CHBENCHMARK_COL_ITEM_I_NAME                   (2)
#define CHBENCHMARK_COL_ITEM_I_PRICE                  (3)
#define CHBENCHMARK_COL_ITEM_I_DATA                   (4)

#define CHBENCHMARK_COL_REGION_R_REGIONKEY            (0)
#define CHBENCHMARK_COL_REGION_R_W_ID                 (1)
#define CHBENCHMARK_COL_REGION_R_NAME                 (2)
#define CHBENCHMARK_COL_REGION_R_COMMENT              (3)

#define CHBENCHMARK_COL_NATION_N_NATION_KEY           (0)
#define CHBENCHMARK_COL_NATION_N_N_W_ID               (1)
#define CHBENCHMARK_COL_NATION_N_REGIONKEY            (2)
#define CHBENCHMARK_COL_NATION_N_COMMENT              (3)

#define CHBENCHMARK_COL_SUPPLIER_S_SUPPLIERKEY        (0)
#define CHBENCHMARK_COL_SUPPLIER_S_W_ID               (1)
#define CHBENCHMARK_COL_SUPPLIER_S_NATIONKEY          (2)
#define CHBENCHMARK_COL_SUPPLIER_S_NAME               (3)
#define CHBENCHMARK_COL_SUPPLIER_S_ADDRESS            (4)
#define CHBENCHMARK_COL_SUPPLIER_S_PHONE              (5)
#define CHBENCHMARK_COL_SUPPLIER_S_ACCTBAL            (6)
#define CHBENCHMARK_COL_SUPPLIER_S_COMMENT            (7)
#define CHBENCHMARK_COL_SUPPLIER_S_MIN_WAREHOUSE      (8)
#define CHBENCHMARK_COL_SUPPLIER_S_MIN_ITEM           (9)
#define CHBENCHMARK_COL_SUPPLIER_S_MIN_QUAN           (10)

#define CHBENCHMARK_VAR_W_ID                          (1001)
#define CHBENCHMARK_VAR_R_ID                          (1096)
#define CHBENCHMARK_VAR_W_NAME                        (1009)
#define CHBENCHMARK_VAR_W_STREET_1                    (1018)
#define CHBENCHMARK_VAR_W_STREET_2                    (1019)
#define CHBENCHMARK_VAR_W_CITY                        (1020)
#define CHBENCHMARK_VAR_W_STATE                       (1021)
#define CHBENCHMARK_VAR_W_ZIP                         (1022)
#define CHBENCHMARK_VAR_H_AMOUNT                      (1002)
#define CHBENCHMARK_VAR_W_TAX                         (1038)

#define CHBENCHMARK_VAR_H_KEY                         (1008)
#define CHBENCHMARK_VAR_O_ID                          (1011)
#define CHBENCHMARK_VAR_O_CARRIER_ID                  (1012)
#define CHBENCHMARK_VAR_O_ENTRY_D                     (1035)
#define CHBENCHMARK_VAR_OL_AMOUNT                     (1013)
#define CHBENCHMARK_VAR_D_NEXT_O_ID                   (1014)
#define CHBENCHMARK_VAR_OL_CNT                        (1015)
#define CHBENCHMARK_VAR_O_ALL_LOCAL                   (1016)
#define CHBENCHMARK_VAR_NO_ID                         (1017)
#define CHBENCHMARK_VAR_D_ID                          (1003)
#define CHBENCHMARK_VAR_D_NAME                        (1010)
#define CHBENCHMARK_VAR_D_STREET_1                    (1030)
#define CHBENCHMARK_VAR_D_STREET_2                    (1031)
#define CHBENCHMARK_VAR_D_CITY                        (1032)
#define CHBENCHMARK_VAR_D_STATE                       (1033)
#define CHBENCHMARK_VAR_D_ZIP                         (1034)
#define CHBENCHMARK_VAR_D_TAX                         (1037)

#define CHBENCHMARK_VAR_C_LAST                        (1004)
#define CHBENCHMARK_VAR_C_W_ID                        (1005)
#define CHBENCHMARK_VAR_C_D_ID                        (1006)
#define CHBENCHMARK_VAR_C_ID                          (1007)
#define CHBENCHMARK_VAR_C_STREET_1                    (1040)
#define CHBENCHMARK_VAR_C_STREET_2                    (1041)
#define CHBENCHMARK_VAR_C_CITY                        (1042)
#define CHBENCHMARK_VAR_C_STATE                       (1043)
#define CHBENCHMARK_VAR_C_ZIP                         (1044)
#define CHBENCHMARK_VAR_C_PHONE                       (1045)
#define CHBENCHMARK_VAR_C_SINCE                       (1046)
#define CHBENCHMARK_VAR_C_CREDIT                      (1047)
#define CHBENCHMARK_VAR_C_CREDIT_LIM                  (1048)
#define CHBENCHMARK_VAR_C_DISCOUNT                    (1049)
#define CHBENCHMARK_VAR_C_BALANCE                     (1050)
#define CHBENCHMARK_VAR_C_FIRST                       (1051)
#define CHBENCHMARK_VAR_C_MIDDLE                      (1052)
#define CHBENCHMARK_VAR_C_ID_LAST                     (1053)
#define CHBENCHMARK_VAR_THRESHOLD                     (1060)
#define CHBENCHMARK_VAR_UNKOWN                        (1061)
#define CHBENCHMARK_VAR_N_PIECE_ALL                   (1080)
#define CHBENCHMARK_VAR_PIECE_WAREHOUSE               (1071)
#define CHBENCHMARK_VAR_PIECE_DISTRICT                (1072)

#define CHBENCHMARK_VAR_I_ID(I)                       (100000 + I)
#define CHBENCHMARK_VAR_I_NAME(I)                     (101000 + I)
#define CHBENCHMARK_VAR_I_PRICE(I)                    (102000 + I)
#define CHBENCHMARK_VAR_I_DATA(I)                     (103000 + I)
#define CHBENCHMARK_VAR_S_DIST_XX(I)                  (104000 + I)
#define CHBENCHMARK_VAR_S_DATA(I)                     (105000 + I)
#define CHBENCHMARK_VAR_S_W_ID(I)                     (106000 + I)
#define CHBENCHMARK_VAR_S_D_ID(I)                     (107000 + I)
#define CHBENCHMARK_VAR_OL_QUANTITY(I)                (108000 + I)
#define CHBENCHMARK_VAR_S_REMOTE_CNT(I)               (109000 + I)
#define CHBENCHMARK_VAR_OL_D_ID(I)                    (110000 + I)
#define CHBENCHMARK_VAR_OL_W_ID(I)                    (111000 + I)
#define CHBENCHMARK_VAR_OL_O_ID(I)                    (112000 + I)
#define CHBENCHMARK_VAR_OL_NUMBER(I)                  (113000 + I)
#define CHBENCHMARK_VAR_OL_DELIVER_D(I)               (114000 + I)
#define CHBENCHMARK_VAR_OL_AMOUNTS(I)                 (115000 + I)
#define CHBENCHMARK_VAR_OL_DIST_INFO(I)               (116000 + I)
#define CHBENCHMARK_VAR_OL_I_ID(I)                    (500000 + I)
#define CHBENCHMARK_VAR_N_KEY(I)                      (600000 + I)
#define CHBENCHMARK_VAR_SU_KEY(I)                     (610000 + I)
#define CHBENCHMARK_VAR_RS_W_ID(I)                    (620000 + I)
#define CHBENCHMARK_VAR_MIN_ITEM(I)                   (630000 + I)
#define CHBENCHMARK_VAR_MIN_QUAN(I)                   (640000 + I)
#define CHBENCHMARK_VAR_OUT_NATION_KEY(I)             (650000 + I)
#define CHBENCHMARK_QUERY_VAR_W_ID(I)                 (660000 + I)
#define CHBENCHMARK_VAR_NUM_N_ID                      (660000)
#define CHBENCHMARK_VAR_NUM_W_ID                      (670000)
#define CHBENCHMARK_VAR_NUM_I_ID                      (680000)
//
//#define CHBENCHMARK_NEW_ORDER_Ith_INDEX_ITEM(i)        (5 + 4 * i)
//#define CHBENCHMARK_NEW_ORDER_Ith_INDEX_IM_STOCK(i)    (6 + 4 * i)
//#define CHBENCHMARK_NEW_ORDER_Ith_INDEX_DEFER_STOCK(i) (7 + 4 * i)
//#define CHBENCHMARK_NEW_ORDER_Ith_INDEX_ORDER_LINE(i)  (8 + 4 * i)
//#define CHBENCHMARK_NEW_ORDER_IS_ITEM_INDEX(i)         ((i >= 5) && (i % 4) == 1)
//#define CHBENCHMARK_NEW_ORDER_IS_IM_STOCK_INDEX(i)     ((i >= 5) && (i % 4) == 2)
//#define CHBENCHMARK_NEW_ORDER_INDEX_ITEM_TO_ORDER_LINE(i)  (i + 3)
//#define CHBENCHMARK_NEW_ORDER_INDEX_ITEM_TO_DEFER_STOCK(i) (i + 2)
//#define CHBENCHMARK_NEW_ORDER_INDEX_IM_STOCK_TO_ORDER_LINE(i) (i + 2)
//#define CHBENCHMARK_NEW_ORDER_INDEX_ITEM_TO_CNT(i)     ((i - 5) / 4)
//#define CHBENCHMARK_NEW_ORDER_INDEX_IM_STOCK_TO_CNT(i) ((i - 6) / 4)

//#define CHBENCHMARK_NEW_ORDER_RI(i)           (15000+i)
//#define CHBENCHMARK_NEW_ORDER_RS(i)           (16000+i)
//#define CHBENCHMARK_NEW_ORDER_WS(i)           (17000+i)
//#define CHBENCHMARK_NEW_ORDER_WOL(i)          (18000+i)

#define CHBENCHMARK_NEW_ORDER_Ith_INDEX_ITEM(i)               (15000 + i)
#define CHBENCHMARK_NEW_ORDER_Ith_INDEX_IM_STOCK(i)           (16000 + i)
#define CHBENCHMARK_NEW_ORDER_Ith_INDEX_DEFER_STOCK(i)        (17000 + i)
#define CHBENCHMARK_NEW_ORDER_Ith_INDEX_ORDER_LINE(i)         (18000 + i)
#define CHBENCHMARK_NEW_ORDER_IS_ITEM_INDEX(i)                ((i >= 15000) && (i < 16000))
#define CHBENCHMARK_NEW_ORDER_IS_IM_STOCK_INDEX(i)            ((i >= 16000) && (i < 17000))
#define CHBENCHMARK_NEW_ORDER_INDEX_ITEM_TO_ORDER_LINE(i)     (i + 3000)
#define CHBENCHMARK_NEW_ORDER_INDEX_ITEM_TO_DEFER_STOCK(i)    (i + 2000)
#define CHBENCHMARK_NEW_ORDER_INDEX_IM_STOCK_TO_ORDER_LINE(i) (i + 2000)
#define CHBENCHMARK_NEW_ORDER_INDEX_ITEM_TO_CNT(i)            ((i - 15000))
#define CHBENCHMARK_NEW_ORDER_INDEX_IM_STOCK_TO_CNT(i)        ((i - 16000))

    extern char CHBENCHMARK_TB_WAREHOUSE[];
    extern char CHBENCHMARK_TB_DISTRICT[];
    extern char CHBENCHMARK_TB_CUSTOMER[];
    extern char CHBENCHMARK_TB_HISTORY[];
    extern char CHBENCHMARK_TB_ORDER[];
    extern char CHBENCHMARK_TB_NEW_ORDER[];
    extern char CHBENCHMARK_TB_ITEM[];
    extern char CHBENCHMARK_TB_STOCK[];
    extern char CHBENCHMARK_TB_ORDER_LINE[];
    extern char CHBENCHMARK_TB_ORDER_C_ID_SECONDARY[];
    extern char CHBENCHMARK_TB_REGION[];
    extern char CHBENCHMARK_TB_NATION[];
    extern char CHBENCHMARK_TB_SUPPLIER[];

    class ChbenchmarkPiece: public Piece {
    protected:
        // new order
        virtual void RegNewOrder();

        // payment
        virtual void RegPayment();

        //void reg_payment_remote();
        //void reg_payment_remote_case1();
        //void reg_payment_remote_case2();

        //void reg_payment_home();
        //void reg_payment_home_case1();
        //void reg_payment_home_case2();

        // order status
        virtual void RegOrderStatus();

        // delivery
        virtual void RegDelivery();

        // stock level
        virtual void RegStockLevel();
        
        // Query1
        virtual void RegQuery1();

        // Query2
        virtual void RegQuery2();

        // Query3
        virtual void RegQuery3();

        // Query4
        virtual void RegQuery4();

        // Query5
        virtual void RegQuery5();

        // Query6
        virtual void RegQuery6();

        // Query7
        virtual void RegQuery7();

        // Query8
        virtual void RegQuery8();

        // Query9
        virtual void RegQuery9();

        // Query10
        virtual void RegQuery10();

        // Query11
        virtual void RegQuery11();

        // Query12
        virtual void RegQuery12();

        // Query13
        virtual void RegQuery13();

        // Query14
        virtual void RegQuery14();

        // Query15
        virtual void RegQuery15();

        // Query16
        virtual void RegQuery16();

        // Query17
        virtual void RegQuery17();

        // Query18
        virtual void RegQuery18();

        // Query19
        virtual void RegQuery19();

        // Query20
        virtual void RegQuery20();

        // Query21
        virtual void RegQuery21();

        // Query22
        virtual void RegQuery22();

    public:
        virtual void reg_all();

        ChbenchmarkPiece();

//pair.first: pieces that may wait for input
//pair.second: pieces that conflict with first
        static std::set<std::pair<innid_t, innid_t>> conflicts_;

        virtual ~ChbenchmarkPiece();

    };



}

#endif
