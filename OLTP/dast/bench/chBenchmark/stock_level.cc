
#include "chopper.h"

namespace rococo {


void ChbenchmarkTxn::StockLevelInit(TxnRequest &req) {

  n_pieces_all_ = 2;
  n_pieces_dispatchable_ = 1;

  stock_level_dep_.w_id = req.input_[CHBENCHMARK_VAR_W_ID].get_i32();
  stock_level_dep_.threshold = req.input_[CHBENCHMARK_VAR_THRESHOLD].get_i32();

  // piece 0, R district
  GetWorkspace(CHBENCHMARK_STOCK_LEVEL_0).keys_ = {
      CHBENCHMARK_VAR_W_ID,
      CHBENCHMARK_VAR_D_ID,
  };
  output_size_[CHBENCHMARK_STOCK_LEVEL_0] = 1;
  p_types_[CHBENCHMARK_STOCK_LEVEL_0] = CHBENCHMARK_STOCK_LEVEL_0;
  status_[CHBENCHMARK_STOCK_LEVEL_0] = DISPATCHABLE;

  // piece 1, R order_line
  GetWorkspace(CHBENCHMARK_STOCK_LEVEL_1).keys_ = {
      CHBENCHMARK_VAR_D_NEXT_O_ID,
      CHBENCHMARK_VAR_W_ID,
      CHBENCHMARK_VAR_D_ID,
  };
  output_size_[CHBENCHMARK_STOCK_LEVEL_1] = 20 * 15 + 1; // 20 orders * 15 order_line per order at most
  p_types_[CHBENCHMARK_STOCK_LEVEL_1] = CHBENCHMARK_STOCK_LEVEL_1;
  status_[CHBENCHMARK_STOCK_LEVEL_1] = WAITING;
  // piece 2 - n, R stock init in stock_level_callback
}

void ChbenchmarkTxn::StockLevelRetry() {
  n_pieces_all_ = 2;
  n_pieces_dispatchable_ = 1;
  // inputs_.resize(n_pieces_all_);
  // output_size_.resize(n_pieces_all_);
  // p_types_.resize(n_pieces_all_);
  // sharding_.resize(n_pieces_all_);
  //  status_.resize(n_pieces_all_);
  status_[CHBENCHMARK_STOCK_LEVEL_0] = DISPATCHABLE;
  status_[CHBENCHMARK_STOCK_LEVEL_1] = WAITING;
}


void ChbenchmarkPiece::RegStockLevel() {
  // Ri district
  INPUT_PIE(CHBENCHMARK_STOCK_LEVEL, CHBENCHMARK_STOCK_LEVEL_0,
            CHBENCHMARK_VAR_W_ID, CHBENCHMARK_VAR_D_ID)
  SHARD_PIE(CHBENCHMARK_STOCK_LEVEL, CHBENCHMARK_STOCK_LEVEL_0,
            CHBENCHMARK_TB_DISTRICT, CHBENCHMARK_VAR_W_ID)
  BEGIN_PIE(CHBENCHMARK_STOCK_LEVEL, CHBENCHMARK_STOCK_LEVEL_0, DF_NO) {
    verify(dtxn != nullptr);
//    verify(input.size() == 2);
    mdb::MultiBlob mb(2);
    //cell_locator_t cl(CHBENCHMARK_TB_DISTRICT, 2);
    mb[0] = cmd.input[CHBENCHMARK_VAR_D_ID].get_blob();
    mb[1] = cmd.input[CHBENCHMARK_VAR_W_ID].get_blob();

    auto tbl_district =  dtxn->GetTable(CHBENCHMARK_TB_DISTRICT);
    mdb::Row *r = dtxn->Query(tbl_district,
                              mb,
                              ROW_DISTRICT);
    dtxn->ReadColumn(r, CHBENCHMARK_COL_DISTRICT_D_NEXT_O_ID,
                     &output[CHBENCHMARK_VAR_D_NEXT_O_ID], TXN_BYPASS);
    *res = SUCCESS;
  } END_PIE

  // Ri order_line
  INPUT_PIE(CHBENCHMARK_STOCK_LEVEL, CHBENCHMARK_STOCK_LEVEL_1,
            CHBENCHMARK_VAR_W_ID, CHBENCHMARK_VAR_D_ID, CHBENCHMARK_VAR_D_NEXT_O_ID)
  SHARD_PIE(CHBENCHMARK_STOCK_LEVEL, CHBENCHMARK_STOCK_LEVEL_1,
            CHBENCHMARK_TB_ORDER_LINE, CHBENCHMARK_VAR_W_ID)
  BEGIN_PIE(CHBENCHMARK_STOCK_LEVEL, CHBENCHMARK_STOCK_LEVEL_1, DF_NO) {
//    verify(input.size() == 3);
    mdb::MultiBlob mbl(4), mbh(4);
    mbl[1] = cmd.input[CHBENCHMARK_VAR_D_ID].get_blob();
    mbh[1] = cmd.input[CHBENCHMARK_VAR_D_ID].get_blob();
    mbl[2] = cmd.input[CHBENCHMARK_VAR_W_ID].get_blob();
    mbh[2] = cmd.input[CHBENCHMARK_VAR_W_ID].get_blob();
    Value ol_o_id_low(cmd.input[CHBENCHMARK_VAR_D_NEXT_O_ID].get_i32() - (i32) 21);
    mbl[0] = ol_o_id_low.get_blob();
    mbh[0] = cmd.input[CHBENCHMARK_VAR_D_NEXT_O_ID].get_blob();
    Value ol_number_low(std::numeric_limits<i32>::max()),
            ol_number_high(std::numeric_limits<i32>::min());
    mbl[3] = ol_number_low.get_blob();
    mbh[3] = ol_number_high.get_blob();

    mdb::ResultSet rs = dtxn->QueryIn(dtxn->GetTable(CHBENCHMARK_TB_ORDER_LINE),
                                      mbl,
                                      mbh,
                                      mdb::ORD_ASC,
                                      cmd.id_);
    Log_debug("tid: %llx, stock_level: piece 1: d_next_o_id: %d, ol_w_id: %d, ol_d_id: %d",
              cmd.root_id_,
              cmd.input[CHBENCHMARK_VAR_D_NEXT_O_ID].get_i32(),
              cmd.input[CHBENCHMARK_VAR_W_ID].get_i32(),
              cmd.input[CHBENCHMARK_VAR_D_ID].get_i32());

    std::vector<mdb::Row *> row_list;
    row_list.reserve(20);

    int i = 0;
    while (i++ < 20 && rs.has_next()) {
        row_list.push_back(rs.next());
    }

//    verify(row_list.size() != 0);

    std::vector<mdb::column_lock_t> column_locks;
    column_locks.reserve(row_list.size());

    for (int i = 0; i < row_list.size(); i++) {
      dtxn->ReadColumn(row_list[i],
                       CHBENCHMARK_COL_ORDER_LINE_OL_I_ID,
                       &output[CHBENCHMARK_VAR_OL_I_ID(i)],
                       TXN_BYPASS);
    }
    output[CHBENCHMARK_VAR_OL_AMOUNT] = Value((int32_t)row_list.size());
//    verify(output_size <= 300);
    *res = SUCCESS;
  } END_PIE

  BEGIN_CB(CHBENCHMARK_STOCK_LEVEL, CHBENCHMARK_STOCK_LEVEL_1)
    ChbenchmarkTxn *chbenchmark_ch = (ChbenchmarkTxn*) ch;
    Log_debug("tid %llx: stock_level: outptu_size: %u",
              chbenchmark_ch->txn_id_, output.size());
    // verify(output_size >= 20 * 5);
    // verify(output_size <= 20 * 15); // TODO fix this verification
    verify(output.size() == (output[CHBENCHMARK_VAR_OL_AMOUNT].get_i32() + 1));
    chbenchmark_ch->n_pieces_all_ += output[CHBENCHMARK_VAR_OL_AMOUNT].get_i32();
    chbenchmark_ch->ws_[CHBENCHMARK_VAR_N_PIECE_ALL] = Value((int32_t)chbenchmark_ch->n_pieces_all_);

    for (int i = 0; i < output[CHBENCHMARK_VAR_OL_AMOUNT].get_i32(); i++) {
      auto pi =  CHBENCHMARK_STOCK_LEVEL_RS(i);
      chbenchmark_ch->GetWorkspace(pi).keys_ = {
          CHBENCHMARK_VAR_OL_I_ID(i),
          CHBENCHMARK_VAR_W_ID,
          CHBENCHMARK_VAR_THRESHOLD
      };
      chbenchmark_ch->status_[pi] = DISPATCHABLE;
      chbenchmark_ch->n_pieces_dispatchable_++;
    }
    return true;
  END_CB


  for (int i = (0); i < (1000); i++) {
    // 1000 is a magical number?
    INPUT_PIE(CHBENCHMARK_STOCK_LEVEL, CHBENCHMARK_STOCK_LEVEL_RS(i),
              CHBENCHMARK_VAR_W_ID, CHBENCHMARK_VAR_OL_I_ID(i), CHBENCHMARK_VAR_THRESHOLD)
    SHARD_PIE(CHBENCHMARK_STOCK_LEVEL, CHBENCHMARK_STOCK_LEVEL_RS(i),
              CHBENCHMARK_TB_STOCK, CHBENCHMARK_VAR_W_ID)
  }
  BEGIN_LOOP_PIE(CHBENCHMARK_STOCK_LEVEL, CHBENCHMARK_STOCK_LEVEL_RS(0), 1000, DF_NO)
//    verify(input.size() == 3);
    Value buf(0);
    mdb::MultiBlob mb(2);
    //cell_locator_t cl(CHBENCHMARK_TB_STOCK, 2);
    mb[0] = cmd.input[CHBENCHMARK_VAR_OL_I_ID(I)].get_blob();
    mb[1] = cmd.input[CHBENCHMARK_VAR_W_ID].get_blob();

    mdb::Row *r = dtxn->Query(dtxn->GetTable(CHBENCHMARK_TB_STOCK), mb, ROW_STOCK);
    dtxn->ReadColumn(r, CHBENCHMARK_COL_STOCK_S_QUANTITY, &buf, TXN_BYPASS);

    if (buf.get_i32() < cmd.input[CHBENCHMARK_VAR_THRESHOLD].get_i32())
        output[CHBENCHMARK_VAR_UNKOWN] = Value((i32) 1);
    else
        output[CHBENCHMARK_VAR_UNKOWN] = Value((i32) 0);

    *res = SUCCESS;
  END_LOOP_PIE
}

} // namespace rococo
