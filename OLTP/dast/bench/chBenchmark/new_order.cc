#include "chopper.h"
#include "generator.h"
#include "piece.h"

namespace rococo {

static uint32_t TXN_TYPE = CHBENCHMARK_NEW_ORDER;
//bool has_dep = true;
static bool has_dep = false;

void ChbenchmarkTxn::NewOrderInit(TxnRequest &req) {
    NewOrderRetry();
}

void ChbenchmarkTxn::NewOrderRetry() {
    status_[CHBENCHMARK_NEW_ORDER_0] = WAITING;
    status_[CHBENCHMARK_NEW_ORDER_1] = WAITING;
    status_[CHBENCHMARK_NEW_ORDER_2] = WAITING;
    status_[CHBENCHMARK_NEW_ORDER_3] = WAITING;
    status_[CHBENCHMARK_NEW_ORDER_4] = WAITING;
    int32_t ol_cnt = ws_[CHBENCHMARK_VAR_OL_CNT].get_i32();
    n_pieces_all_ = 5 + 4 * ol_cnt;
    n_pieces_dispatchable_ = 0;
    n_pieces_dispatch_acked_ = 0;
    n_pieces_dispatched_ = 0;
    for (int i = 0; i < ol_cnt; i++) {
        status_[CHBENCHMARK_NEW_ORDER_RI(i)] = WAITING;
        status_[CHBENCHMARK_NEW_ORDER_RS(i)] = WAITING;
        status_[CHBENCHMARK_NEW_ORDER_WS(i)] = WAITING;
        status_[CHBENCHMARK_NEW_ORDER_WOL(i)] = WAITING;
    }
    CheckReady();
}

void ChbenchmarkPiece::RegNewOrder() {
    // Ri & W district
    INPUT_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_0,
              CHBENCHMARK_VAR_W_ID, CHBENCHMARK_VAR_D_ID)
    OUTPUT_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_0,
               CHBENCHMARK_VAR_O_ID, CHBENCHMARK_VAR_D_TAX)
    SHARD_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_0,
              CHBENCHMARK_TB_DISTRICT, CHBENCHMARK_VAR_W_ID);
    BEGIN_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_0,
              DF_REAL) {
        verify(cmd.input.size() >= 2);
        verify(cmd.input[CHBENCHMARK_VAR_W_ID].get_i32() >= 0);
        mdb::MultiBlob mb(2);
        mb[0] = cmd.input[CHBENCHMARK_VAR_D_ID].get_blob();
        mb[1] = cmd.input[CHBENCHMARK_VAR_W_ID].get_blob();
        Log_info("new order piece 1 d_id: %x w_id: %x",
                 cmd.input[CHBENCHMARK_VAR_D_ID].get_i32(),
                 cmd.input[CHBENCHMARK_VAR_W_ID].get_i32());
        mdb::Row *row_district = dtxn->Query(dtxn->GetTable(CHBENCHMARK_TB_DISTRICT),
                                             mb,
                                             ROW_DISTRICT);
        Value buf(0);
        // R district
        dtxn->ReadColumn(row_district,
                         CHBENCHMARK_COL_DISTRICT_D_TAX,
                         &output[CHBENCHMARK_VAR_D_TAX],
                         TXN_BYPASS);
        dtxn->ReadColumn(row_district,
                         CHBENCHMARK_COL_DISTRICT_D_NEXT_O_ID,
                         &buf,
                         TXN_BYPASS);// read d_next_o_id
        output[CHBENCHMARK_VAR_O_ID] = buf;
        Log_info("new order piece 1 o_id: %x",
                 output[CHBENCHMARK_VAR_O_ID].get_i32());
        // read d_next_o_id, increment by 1
        buf.set_i32((i32) (buf.get_i32() + 1));
        dtxn->WriteColumn(row_district,
                          CHBENCHMARK_COL_DISTRICT_D_NEXT_O_ID,
                          buf,
                          TXN_DEFERRED);
        return;
    }
    END_PIE

    // R warehouse
    INPUT_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_1,
              CHBENCHMARK_VAR_W_ID)
    OUTPUT_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_1,
               CHBENCHMARK_VAR_W_TAX)
    SHARD_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_1,
              CHBENCHMARK_TB_DISTRICT, CHBENCHMARK_VAR_W_ID);
    BEGIN_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_1, DF_REAL) {
        verify(cmd.input.size() >= 1);
        Log_info("CHBENCHMARK_NEW_ORDER, piece: %d w_id: %x", CHBENCHMARK_NEW_ORDER_1, cmd.input[CHBENCHMARK_VAR_W_ID].get_i32());
        mdb::Row *row_warehouse = dtxn->Query(dtxn->GetTable(CHBENCHMARK_TB_WAREHOUSE),
                                              cmd.input[CHBENCHMARK_VAR_W_ID].get_blob(),
                                              ROW_WAREHOUSE);
        // R warehouse
        dtxn->ReadColumn(row_warehouse, CHBENCHMARK_COL_WAREHOUSE_W_TAX,
                         &output[CHBENCHMARK_VAR_W_TAX], TXN_BYPASS);// read w_tax
        return;
    }
    END_PIE

    // R customer //XXX either i or d is ok
    INPUT_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_2,
              CHBENCHMARK_VAR_W_ID, CHBENCHMARK_VAR_D_ID, CHBENCHMARK_VAR_C_ID)
    OUTPUT_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_1,
               CHBENCHMARK_VAR_C_LAST, CHBENCHMARK_VAR_C_CREDIT, CHBENCHMARK_VAR_C_DISCOUNT)
    SHARD_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_2,
              CHBENCHMARK_TB_CUSTOMER, CHBENCHMARK_VAR_W_ID)
    BEGIN_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_2, DF_REAL) {
        verify(cmd.input.size() >= 3);
        mdb::MultiBlob mb(3);
        mb[0] = cmd.input[CHBENCHMARK_VAR_C_ID].get_blob();
        mb[1] = cmd.input[CHBENCHMARK_VAR_D_ID].get_blob();
        mb[2] = cmd.input[CHBENCHMARK_VAR_W_ID].get_blob();
        auto table = dtxn->GetTable(CHBENCHMARK_TB_CUSTOMER);
        mdb::Row *row_customer = dtxn->Query(table, mb, ROW_CUSTOMER);
        // R customer
        Log_info("CHBENCHMARK_NEW_ORDER, piece: %d c_id: %d d_id: %d w_id: %d",
                 CHBENCHMARK_NEW_ORDER_2,
                 cmd.input[CHBENCHMARK_VAR_C_ID].get_i32(),
                 cmd.input[CHBENCHMARK_VAR_D_ID].get_i32(),
                 cmd.input[CHBENCHMARK_VAR_W_ID].get_i32());
        dtxn->ReadColumn(row_customer, CHBENCHMARK_COL_CUSTOMER_C_LAST,
                         &output[CHBENCHMARK_VAR_C_LAST], TXN_BYPASS);
        dtxn->ReadColumn(row_customer, CHBENCHMARK_COL_CUSTOMER_C_CREDIT,
                         &output[CHBENCHMARK_VAR_C_CREDIT], TXN_BYPASS);
        dtxn->ReadColumn(row_customer, CHBENCHMARK_COL_CUSTOMER_C_DISCOUNT,
                         &output[CHBENCHMARK_VAR_C_DISCOUNT], TXN_BYPASS);

        return;
    }
    END_PIE

    // W order
    INPUT_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_3,
              CHBENCHMARK_VAR_W_ID, CHBENCHMARK_VAR_D_ID, CHBENCHMARK_VAR_C_ID, CHBENCHMARK_VAR_O_ID,
              CHBENCHMARK_VAR_O_CARRIER_ID, CHBENCHMARK_VAR_OL_CNT, CHBENCHMARK_VAR_O_ALL_LOCAL)
    SHARD_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_3,
              CHBENCHMARK_TB_ORDER, CHBENCHMARK_VAR_W_ID)
    BEGIN_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_3, DF_REAL) {
        verify(cmd.input.size() >= 6);
        Log_info("CHBENCHMARK_NEW_ORDER, piece: %d", CHBENCHMARK_NEW_ORDER_3);
        i32 oi = 0;
        mdb::Table *tbl = dtxn->GetTable(CHBENCHMARK_TB_ORDER);
        //    if (tbl == nullptr){
        //      Log_info("null ptr");
        //    }else{
        //      Log_info("table name %s", tbl->Name().c_str());
        //    }
        mdb::MultiBlob mb(3);
        mb[0] = cmd.input[CHBENCHMARK_VAR_D_ID].get_blob();
        mb[1] = cmd.input[CHBENCHMARK_VAR_W_ID].get_blob();
        mb[2] = cmd.input[CHBENCHMARK_VAR_C_ID].get_blob();
        mdb::Row *r = dtxn->Query(dtxn->GetTable(CHBENCHMARK_TB_ORDER_C_ID_SECONDARY),
                                  mb,
                                  ROW_ORDER_SEC);

        verify(r);
        verify(r->schema_);
        // W order
        //Log_info("h");
        std::vector<Value> row_data = {
                cmd.input.at(CHBENCHMARK_VAR_O_ID),
                cmd.input.at(CHBENCHMARK_VAR_D_ID),
                cmd.input[CHBENCHMARK_VAR_W_ID],
                cmd.input[CHBENCHMARK_VAR_C_ID],
                Value(std::to_string(time(NULL))),// o_entry_d
                cmd.input[CHBENCHMARK_VAR_O_CARRIER_ID],
                cmd.input[CHBENCHMARK_VAR_OL_CNT],
                cmd.input[CHBENCHMARK_VAR_O_ALL_LOCAL]};
        //Log_info("hh");
        // Log_info("oid:%d did:%d wid:%d cid:%d carrierid:%d ol_cnt:%d all:%d",
        //          cmd.input.at(CHBENCHMARK_VAR_O_ID).get_i32(), cmd.input.at(CHBENCHMARK_VAR_D_ID).get_i32(),
        //          cmd.input[CHBENCHMARK_VAR_W_ID].get_i32(), cmd.input[CHBENCHMARK_VAR_C_ID].get_i32(),
        //          cmd.input[CHBENCHMARK_VAR_O_CARRIER_ID].get_i32(), cmd.input[CHBENCHMARK_VAR_OL_CNT].get_i32(),
        //          cmd.input[CHBENCHMARK_VAR_O_ALL_LOCAL].get_i32());
        CREATE_ROW(tbl->schema(), row_data);
        verify(r->schema_);
        mdb::Txn *txn = dtxn->mdb_txn();
        dtxn->InsertRow(tbl, r);
        //    r = dtxn->Query(dtxn->GetTable(CHBENCHMARK_TB_ORDER_C_ID_SECONDARY),
        //                    mb,
        //                    ROW_ORDER_SEC);
        //dtxn->WriteColumn(r, 3, cmd.input[CHBENCHMARK_VAR_W_ID], TXN_BYPASS);
        return;
    }
    END_PIE

    // W new_order
    INPUT_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_4,
              CHBENCHMARK_VAR_W_ID, CHBENCHMARK_VAR_D_ID, CHBENCHMARK_VAR_O_ID)
    SHARD_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_4,
              CHBENCHMARK_TB_NEW_ORDER, CHBENCHMARK_VAR_W_ID);
    BEGIN_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_4, DF_REAL) {
        verify(cmd.input.size() >= 3);
        Log_info("CHBENCHMARK_NEW_ORDER, piece: %d", CHBENCHMARK_NEW_ORDER_4);

        mdb::Table *tbl = dtxn->GetTable(CHBENCHMARK_TB_NEW_ORDER);
        mdb::Row *r = NULL;
        // W new_order
        std::vector<Value> row_data({
                cmd.input[CHBENCHMARK_VAR_O_ID],
                cmd.input[CHBENCHMARK_VAR_D_ID],// o_d_id
                cmd.input[CHBENCHMARK_VAR_W_ID] // o_w_id
        });
        CREATE_ROW(tbl->schema(), row_data);
        mdb::Txn *txn = dtxn->mdb_txn();
        dtxn->InsertRow(tbl, r);
        *res = SUCCESS;
        return;
    }
    END_PIE

    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_RI(i),
                  CHBENCHMARK_TB_ITEM, CHBENCHMARK_VAR_I_ID(i))
        INPUT_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_RI(i), CHBENCHMARK_VAR_I_ID(i))
    }

    BEGIN_LOOP_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_RI(0), 1000, DF_NO)
    verify(cmd.input.size() >= 1);
    Log_info("CHBENCHMARK_NEW_ORDER, piece: %d", CHBENCHMARK_NEW_ORDER_RI(I));
    auto tbl_item = dtxn->GetTable(CHBENCHMARK_TB_ITEM);
    mdb::Row *row_item = dtxn->Query(tbl_item,
                                     cmd.input[CHBENCHMARK_VAR_I_ID(I)].get_blob(),
                                     ROW_ITEM);
    // Ri item
    dtxn->ReadColumn(row_item,
                     CHBENCHMARK_COL_ITEM_I_NAME,
                     &output[CHBENCHMARK_VAR_I_NAME(I)],
                     TXN_INSTANT);
    dtxn->ReadColumn(row_item,
                     CHBENCHMARK_COL_ITEM_I_PRICE,
                     &output[CHBENCHMARK_VAR_I_PRICE(I)],
                     TXN_INSTANT);
    dtxn->ReadColumn(row_item,
                     CHBENCHMARK_COL_ITEM_I_DATA,
                     &output[CHBENCHMARK_VAR_I_DATA(I)],
                     TXN_INSTANT);

    *res = SUCCESS;
    return;
    END_LOOP_PIE


    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        if (has_dep) {
            SHARD_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_RS(i),
                      CHBENCHMARK_TB_STOCK, CHBENCHMARK_VAR_S_W_ID(i))
            INPUT_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_RS(i),
                      CHBENCHMARK_VAR_D_ID, CHBENCHMARK_VAR_I_ID(i), CHBENCHMARK_VAR_S_W_ID(i))
        } else {
            SHARD_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_RS(i),
                      CHBENCHMARK_TB_STOCK, CHBENCHMARK_VAR_W_ID)
            INPUT_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_RS(i),
                      CHBENCHMARK_VAR_D_ID, CHBENCHMARK_VAR_I_ID(i), CHBENCHMARK_VAR_W_ID)
        }
    }

    BEGIN_LOOP_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_RS(0), 1000, DF_NO)
    verify(cmd.input.size() >= 3);
    Log_info("CHBENCHMARK_NEW_ORDER, piece: %d", CHBENCHMARK_NEW_ORDER_RS(I));
    mdb::MultiBlob mb(2);
    int32_t w_id;
    mb[0] = cmd.input[CHBENCHMARK_VAR_I_ID(I)].get_blob();
    if (has_dep) {
        mb[1] = cmd.input[CHBENCHMARK_VAR_S_W_ID(I)].get_blob();
        w_id = cmd.input[CHBENCHMARK_VAR_S_W_ID(I)].get_i32();
    } else {
        mb[1] = cmd.input[CHBENCHMARK_VAR_W_ID].get_blob();
        w_id = cmd.input[CHBENCHMARK_VAR_W_ID].get_i32();
    }
    int32_t i_id = cmd.input[CHBENCHMARK_VAR_I_ID(I)].get_i32();
    Log_debug("new order read stock. item_id: %x, s_w_id: %x",
              i_id,
              w_id);
    auto tbl_stock = dtxn->GetTable(CHBENCHMARK_TB_STOCK);
    mdb::Row *r = dtxn->Query(tbl_stock, mb, ROW_STOCK);
    verify(r->schema_);
    //i32 s_dist_col = 3 + input[2].get_i32();
    // Ri stock
    // FIXME compress all s_dist_xx into one column
    dtxn->ReadColumn(r, CHBENCHMARK_COL_STOCK_S_DIST_XX,
                     &output[CHBENCHMARK_VAR_OL_DIST_INFO(I)],
                     TXN_INSTANT);// 0 ==> s_dist_xx
    dtxn->ReadColumn(r, CHBENCHMARK_COL_STOCK_S_DATA,
                     &output[CHBENCHMARK_VAR_S_DATA(I)],
                     TXN_INSTANT);// 1 ==> s_data
    *res = SUCCESS;
    return;
    END_LOOP_PIE

    for (int i = 0; i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_WS(i),
                  CHBENCHMARK_TB_STOCK, CHBENCHMARK_VAR_S_W_ID(i))
        INPUT_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_WS(i),
                  CHBENCHMARK_VAR_I_ID(i), CHBENCHMARK_VAR_S_W_ID(i),
                  CHBENCHMARK_VAR_OL_QUANTITY(i), CHBENCHMARK_VAR_S_REMOTE_CNT(i))
    }

    BEGIN_LOOP_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_WS(0), 1000, DF_REAL)
    verify(cmd.input.size() >= 4);
    Log_info("CHBENCHMARK_NEW_ORDER, piece: %d", CHBENCHMARK_NEW_ORDER_WS(I));
    mdb::Row *r = NULL;
    mdb::MultiBlob mb(2);
    mb[0] = cmd.input[CHBENCHMARK_VAR_I_ID(I)].get_blob();
    mb[1] = cmd.input[CHBENCHMARK_VAR_S_W_ID(I)].get_blob();

    r = dtxn->Query(dtxn->GetTable(CHBENCHMARK_TB_STOCK), mb, ROW_STOCK_TEMP);
    verify(r->schema_);
    // Ri stock
    Value buf(0);
    dtxn->ReadColumn(r, CHBENCHMARK_COL_STOCK_S_QUANTITY, &buf, TXN_DEFERRED);
    int32_t new_ol_quantity = buf.get_i32() -
                              cmd.input[CHBENCHMARK_VAR_OL_QUANTITY(I)].get_i32();

    dtxn->ReadColumn(r, CHBENCHMARK_COL_STOCK_S_YTD, &buf, TXN_DEFERRED);
    Value new_s_ytd(buf.get_i32() +
                    cmd.input[CHBENCHMARK_VAR_OL_QUANTITY(I)].get_i32());

    dtxn->ReadColumn(r, CHBENCHMARK_COL_STOCK_S_ORDER_CNT, &buf, TXN_DEFERRED);
    Value new_s_order_cnt((i32) (buf.get_i32() + 1));

    dtxn->ReadColumn(r, CHBENCHMARK_COL_STOCK_S_REMOTE_CNT, &buf, TXN_DEFERRED);
    Value new_s_remote_cnt(buf.get_i32() +
                           cmd.input[CHBENCHMARK_VAR_S_REMOTE_CNT(I)].get_i32());

    if (new_ol_quantity < 10)
        new_ol_quantity += 91;
    Value new_ol_quantity_value(new_ol_quantity);

    dtxn->WriteColumns(r,
                       {CHBENCHMARK_COL_STOCK_S_QUANTITY,
                        CHBENCHMARK_COL_STOCK_S_YTD,
                        CHBENCHMARK_COL_STOCK_S_ORDER_CNT,
                        CHBENCHMARK_COL_STOCK_S_REMOTE_CNT},
                       {new_ol_quantity_value,
                        new_s_ytd,
                        new_s_order_cnt,
                        new_s_remote_cnt},
                       TXN_DEFERRED);
    *res = SUCCESS;
    return;
    END_LOOP_PIE

    for (int i = 0; i < 1000; i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_WOL(i),
                  CHBENCHMARK_TB_ORDER_LINE, CHBENCHMARK_VAR_W_ID)
        INPUT_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_WOL(i),
                  CHBENCHMARK_VAR_I_ID(i),
                  CHBENCHMARK_VAR_O_ID, CHBENCHMARK_VAR_S_W_ID(i),
                  CHBENCHMARK_VAR_W_ID, CHBENCHMARK_VAR_D_ID,
                  CHBENCHMARK_VAR_OL_DIST_INFO(i), CHBENCHMARK_VAR_OL_QUANTITY(i),
                  CHBENCHMARK_VAR_OL_NUMBER(i), CHBENCHMARK_VAR_OL_DELIVER_D(i))
    }

    BEGIN_LOOP_PIE(CHBENCHMARK_NEW_ORDER, CHBENCHMARK_NEW_ORDER_WOL(0), 1000, DF_REAL) {
        verify(cmd.input.size() >= 9);
        Log_info("CHBENCHMARK_NEW_ORDER, piece: %d", CHBENCHMARK_NEW_ORDER_WOL(I));

        mdb::Table *tbl = dtxn->GetTable(CHBENCHMARK_TB_ORDER_LINE);
        mdb::Row *r = NULL;

        double am = (double) cmd.input[CHBENCHMARK_VAR_OL_QUANTITY(I)].get_i32();
        Value amount = Value(am);
        Value xxx = Value("");
        CREATE_ROW(tbl->schema(), vector<Value>({
                                          cmd.input[CHBENCHMARK_VAR_O_ID],
                                          cmd.input[CHBENCHMARK_VAR_D_ID],
                                          cmd.input[CHBENCHMARK_VAR_W_ID],
                                          cmd.input[CHBENCHMARK_VAR_OL_NUMBER(I)],
                                          cmd.input[CHBENCHMARK_VAR_I_ID(I)],
                                          cmd.input[CHBENCHMARK_VAR_S_W_ID(I)],
                                          cmd.input[CHBENCHMARK_VAR_OL_DELIVER_D(I)],
                                          cmd.input[CHBENCHMARK_VAR_OL_QUANTITY(I)],
                                          amount,
                                          cmd.input[CHBENCHMARK_VAR_OL_DIST_INFO(I)],
                                  }));
        mdb::Txn *txn = dtxn->mdb_txn();
        dtxn->InsertRow(tbl, r);
        *res = SUCCESS;
        return;
    }
    END_LOOP_PIE
}
}// namespace rococo
