#include "chopper.h"
#include "deptran/__dep__.h"

namespace rococo {

void ChbenchmarkTxn::DeliveryInit(TxnRequest &req) {
    n_pieces_all_ = 4;

    // piece 0, Ri & W new_order
    output_size_[CHBENCHMARK_DELIVERY_0] = 2;
    // piece 1, Ri & W order
    output_size_[CHBENCHMARK_DELIVERY_1] = 1;
    // piece 2, Ri & W order_line
    output_size_[CHBENCHMARK_DELIVERY_2] = 1;
    // piece 3, W customer
    output_size_[CHBENCHMARK_DELIVERY_3] = 0;

    p_types_[CHBENCHMARK_DELIVERY_0] = CHBENCHMARK_DELIVERY_0;
    p_types_[CHBENCHMARK_DELIVERY_1] = CHBENCHMARK_DELIVERY_1;
    p_types_[CHBENCHMARK_DELIVERY_2] = CHBENCHMARK_DELIVERY_2;
    p_types_[CHBENCHMARK_DELIVERY_3] = CHBENCHMARK_DELIVERY_3;

    status_[CHBENCHMARK_DELIVERY_0] = WAITING;
    status_[CHBENCHMARK_DELIVERY_1] = WAITING;
    status_[CHBENCHMARK_DELIVERY_2] = WAITING;
    status_[CHBENCHMARK_DELIVERY_3] = WAITING;
    CheckReady();
}

void ChbenchmarkTxn::DeliveryRetry() {
    status_[CHBENCHMARK_DELIVERY_0] = WAITING;
    status_[CHBENCHMARK_DELIVERY_1] = WAITING;
    status_[CHBENCHMARK_DELIVERY_2] = WAITING;
    status_[CHBENCHMARK_DELIVERY_3] = WAITING;
    CheckReady();
}


void ChbenchmarkPiece::RegDelivery() {
    // Ri & W new_order
    INPUT_PIE(CHBENCHMARK_DELIVERY, CHBENCHMARK_DELIVERY_0,
              CHBENCHMARK_VAR_W_ID, CHBENCHMARK_VAR_D_ID, CHBENCHMARK_VAR_O_CARRIER_ID)
    OUTPUT_PIE(CHBENCHMARK_DELIVERY, CHBENCHMARK_DELIVERY_0,
               CHBENCHMARK_VAR_O_ID)
    SHARD_PIE(CHBENCHMARK_DELIVERY, CHBENCHMARK_DELIVERY_0,
              CHBENCHMARK_TB_NEW_ORDER, CHBENCHMARK_VAR_W_ID)
    BEGIN_PIE(CHBENCHMARK_DELIVERY, CHBENCHMARK_DELIVERY_0, DF_REAL) {
        // this is a little bit tricky, the first half will do most of the job,
        // removing the row from the table, but it won't actually release the
        // resource. And the bottom half is in charge of release the resource,
        // including the vertex entry

        Log_debug("CHBENCHMARK_DELIVERY, piece: %d", CHBENCHMARK_DELIVERY_0);
        verify(cmd.input.size() >= 3);
        Value buf;
        //cell_locator_t cl(CHBENCHMARK_TB_NEW_ORDER, 3);
        mdb::Row *r = NULL;
        mdb::Table *tbl = dtxn->GetTable(CHBENCHMARK_TB_NEW_ORDER);

        mdb::MultiBlob mbl(3), mbh(3);
        mbl[1] = cmd.input[CHBENCHMARK_VAR_D_ID].get_blob();
        mbh[1] = cmd.input[CHBENCHMARK_VAR_D_ID].get_blob();
        mbl[2] = cmd.input[CHBENCHMARK_VAR_W_ID].get_blob();
        mbh[2] = cmd.input[CHBENCHMARK_VAR_W_ID].get_blob();
        Value no_o_id_low(2101),
                no_o_id_high(2999);
        mbl[0] = no_o_id_low.get_blob();
        mbh[0] = no_o_id_high.get_blob();

        mdb::ResultSet rs = dtxn->QueryIn(tbl,
                                          mbl,
                                          mbh,
                                          mdb::ORD_ASC,
                                          RS_NEW_ORDER);
        Value o_id(0);
        if (rs.has_next()) {
            r = rs.next();
            //      TPL_KISS_ROW(r);
            dtxn->ReadColumn(r, CHBENCHMARK_COL_NEW_ORDER_NO_O_ID, &o_id, TXN_DEFERRED);
            output[CHBENCHMARK_VAR_O_ID] = o_id;
        }
        //      verify(0);
        //      TPL_KISS_NONE;
        output[CHBENCHMARK_VAR_O_ID] = Value(2101);
        // TODO FIXME
        //    if (r) {
        //      mdb::Txn *txn = dtxn->mdb_txn();
        //      txn->remove_row(tbl, r);
        //    }

        *res = SUCCESS;
        return;
    }
    END_PIE

    // Ri & W order
    INPUT_PIE(CHBENCHMARK_DELIVERY, CHBENCHMARK_DELIVERY_1,
              CHBENCHMARK_VAR_W_ID, CHBENCHMARK_VAR_D_ID,
              CHBENCHMARK_VAR_O_ID, CHBENCHMARK_VAR_O_CARRIER_ID)
    OUTPUT_PIE(CHBENCHMARK_DELIVERY, CHBENCHMARK_DELIVERY_1,
               CHBENCHMARK_VAR_C_ID)
    SHARD_PIE(CHBENCHMARK_DELIVERY, CHBENCHMARK_DELIVERY_1,
              CHBENCHMARK_TB_ORDER, CHBENCHMARK_VAR_W_ID)
    BEGIN_PIE(CHBENCHMARK_DELIVERY, CHBENCHMARK_DELIVERY_1, DF_NO) {
        Log_debug("CHBENCHMARK_DELIVERY, piece: %d", CHBENCHMARK_DELIVERY_1);
        verify(cmd.input.size() >= 4);
        mdb::Txn *txn = dtxn->mdb_txn_;
        mdb::MultiBlob mb(3);
        //cell_locator_t cl(CHBENCHMARK_TB_ORDER, 3);
        mb[0] = cmd.input[CHBENCHMARK_VAR_O_ID].get_blob();
        mb[1] = cmd.input[CHBENCHMARK_VAR_D_ID].get_blob();
        mb[2] = cmd.input[CHBENCHMARK_VAR_W_ID].get_blob();
        Log_info("order status d_id: %d w_id: %d o_id: %d",
                 cmd.input[CHBENCHMARK_VAR_D_ID].get_i32(),
                 cmd.input[CHBENCHMARK_VAR_W_ID].get_i32(),
                 cmd.input[CHBENCHMARK_VAR_O_ID].get_i32());
        //    Log_info("Delivery: o_d_id: %d, o_w_id: %d, o_id: %d, hash: %u", input[2].get_i32(), input[1].get_i32(), input[0].get_i32(), mdb::MultiBlob::hash()(cl.primary_key));
        auto tbl_order = txn->get_table(CHBENCHMARK_TB_ORDER);
        mdb::Row *row_order = dtxn->Query(tbl_order, mb, ROW_ORDER);
        dtxn->ReadColumn(row_order,
                         CHBENCHMARK_COL_ORDER_O_C_ID,
                         &output[CHBENCHMARK_VAR_C_ID],
                         TXN_BYPASS);// read o_c_id
        dtxn->WriteColumn(row_order,
                          CHBENCHMARK_COL_ORDER_O_CARRIER_ID,
                          cmd.input[CHBENCHMARK_VAR_O_CARRIER_ID],
                          TXN_DEFERRED);// write o_carrier_id
        return;
    }
    END_PIE

    //   Ri & W order_line
    INPUT_PIE(CHBENCHMARK_DELIVERY, CHBENCHMARK_DELIVERY_2,
              CHBENCHMARK_VAR_W_ID, CHBENCHMARK_VAR_D_ID, CHBENCHMARK_VAR_O_ID)
    SHARD_PIE(CHBENCHMARK_DELIVERY, CHBENCHMARK_DELIVERY_2,
              CHBENCHMARK_TB_ORDER_LINE, CHBENCHMARK_VAR_W_ID)
    BEGIN_PIE(CHBENCHMARK_DELIVERY, CHBENCHMARK_DELIVERY_2, DF_NO) {
        Log_debug("CHBENCHMARK_DELIVERY, piece: %d", CHBENCHMARK_DELIVERY_2);
        verify(cmd.input.size() >= 3);
        //        mdb::Txn *txn = DTxnMgr::get_sole_mgr()->get_mdb_txn(header);
        mdb::MultiBlob mbl = mdb::MultiBlob(4);
        mdb::MultiBlob mbh = mdb::MultiBlob(4);
        //    mdb::MultiBlob mbl(4), mbh(4);
        mbl[1] = cmd.input[CHBENCHMARK_VAR_D_ID].get_blob();
        mbh[1] = cmd.input[CHBENCHMARK_VAR_D_ID].get_blob();
        mbl[2] = cmd.input[CHBENCHMARK_VAR_W_ID].get_blob();
        mbh[2] = cmd.input[CHBENCHMARK_VAR_W_ID].get_blob();
        mbl[0] = cmd.input[CHBENCHMARK_VAR_O_ID].get_blob();
        mbh[0] = cmd.input[CHBENCHMARK_VAR_O_ID].get_blob();
        Value ol_number_low(std::numeric_limits<i32>::min()),
                ol_number_high(std::numeric_limits<i32>::max());
        mbl[3] = ol_number_low.get_blob();
        mbh[3] = ol_number_high.get_blob();

        mdb::ResultSet rs_ol = dtxn->QueryIn(dtxn->GetTable(CHBENCHMARK_TB_ORDER_LINE),
                                             mbl,
                                             mbh,
                                             mdb::ORD_ASC,
                                             RS_ORDER_LINE);
        mdb::Row *row_ol = nullptr;
        //                cell_locator_t cl(CHBENCHMARK_TB_ORDER_LINE, 4);
        //                cl.primary_key[0] = input[2].get_blob();
        //                cl.primary_key[1] = input[1].get_blob();
        //                cl.primary_key[2] = input[0].get_blob();

        std::vector<mdb::Row *> row_list;
        row_list.reserve(15);
        while (rs_ol.has_next()) {
            row_list.push_back(rs_ol.next());
        }

        std::vector<mdb::column_lock_t> column_locks;
        column_locks.reserve(2 * row_list.size());

        int i = 0;
        double ol_amount_buf = 0.0;
        row_ol = row_list[i++];
        Value buf(0.0);
        dtxn->ReadColumn(row_ol, CHBENCHMARK_COL_ORDER_LINE_OL_AMOUNT,
                         &buf, TXN_DEFERRED);// read ol_amount
        ol_amount_buf += buf.get_double();
        dtxn->WriteColumn(row_ol, CHBENCHMARK_COL_ORDER_LINE_OL_DELIVERY_D,
                          Value(std::to_string(time(NULL))),
                          TXN_DEFERRED);
        // while (i < row_list.size()) {
        //   row_ol = row_list[i++];
        //   Value buf(0.0);
        //   dtxn->ReadColumn(row_ol, CHBENCHMARK_COL_ORDER_LINE_OL_AMOUNT,
        //                    &buf, TXN_DEFERRED); // read ol_amount
        //   ol_amount_buf += buf.get_double();
        //   dtxn->WriteColumn(row_ol, CHBENCHMARK_COL_ORDER_LINE_OL_DELIVERY_D,
        //                     Value(std::to_string(time(NULL))),
        //                     TXN_DEFERRED);
        // }
        output[CHBENCHMARK_VAR_OL_AMOUNT] = Value(ol_amount_buf);

        *res = SUCCESS;
    }
    END_PIE

    // W customer
    INPUT_PIE(CHBENCHMARK_DELIVERY, CHBENCHMARK_DELIVERY_3,
              CHBENCHMARK_VAR_W_ID, CHBENCHMARK_VAR_D_ID,
              CHBENCHMARK_VAR_C_ID, CHBENCHMARK_VAR_OL_AMOUNT)
    SHARD_PIE(CHBENCHMARK_DELIVERY, CHBENCHMARK_DELIVERY_3,
              CHBENCHMARK_TB_CUSTOMER, CHBENCHMARK_VAR_W_ID)
    BEGIN_PIE(CHBENCHMARK_DELIVERY, CHBENCHMARK_DELIVERY_3, DF_REAL) {
        Log_debug("CHBENCHMARK_DELIVERY, piece: %d", CHBENCHMARK_DELIVERY_3);
        verify(cmd.input.size() >= 4);
        mdb::Row *row_customer = NULL;
        mdb::MultiBlob mb = mdb::MultiBlob(3);
        //cell_locator_t cl(CHBENCHMARK_TB_CUSTOMER, 3);
        mb[0] = cmd.input[CHBENCHMARK_VAR_C_ID].get_blob();
        mb[1] = cmd.input[CHBENCHMARK_VAR_D_ID].get_blob();
        mb[2] = cmd.input[CHBENCHMARK_VAR_W_ID].get_blob();

        auto tbl_customer = dtxn->GetTable(CHBENCHMARK_TB_CUSTOMER);
        row_customer = dtxn->Query(tbl_customer, mb, ROW_CUSTOMER);
        Value buf = Value(0.0);
        dtxn->ReadColumn(row_customer, CHBENCHMARK_COL_CUSTOMER_C_BALANCE,
                         &buf, TXN_DEFERRED);
        buf.set_double(buf.get_double() +
                       cmd.input[CHBENCHMARK_VAR_OL_AMOUNT].get_double());
        dtxn->WriteColumn(row_customer, CHBENCHMARK_COL_CUSTOMER_C_BALANCE,
                          buf, TXN_DEFERRED);
        dtxn->ReadColumn(row_customer, CHBENCHMARK_COL_CUSTOMER_C_DELIVERY_CNT,
                         &buf, TXN_BYPASS);
        buf.set_i32(buf.get_i32() + (i32) 1);
        dtxn->WriteColumn(row_customer, CHBENCHMARK_COL_CUSTOMER_C_DELIVERY_CNT,
                          buf, TXN_DEFERRED);
        *res = SUCCESS;
    }
    END_PIE
}

}// namespace rococo
