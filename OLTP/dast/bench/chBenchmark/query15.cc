#include "chopper.h"
#include "generator.h"
#include "piece.h"

namespace rococo {

void ChbenchmarkTxn::Query15Init(TxnRequest &req) {
    Query15Retry();
}

void ChbenchmarkTxn::Query15Retry() {
    for (int i = 0; i < Config::GetConfig()->GetNumPartition(); i++) {
        status_[CHBENCHMARK_QUERY15_OL(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY15_ST(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY15_SP(i)] = DISPATCHABLE;
    }
    n_pieces_all_ = Config::GetConfig()->GetNumPartition();//one partition one warehouse
    n_pieces_dispatchable_ = Config::GetConfig()->GetNumPartition();
    n_pieces_dispatch_acked_ = 0;
    n_pieces_dispatched_ = 0;
    CheckReady();
}

void ChbenchmarkPiece::RegQuery15() {
    //read all order line shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY15, CHBENCHMARK_QUERY15_OL(i),
                  CHBENCHMARK_TB_ORDER_LINE, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY15, CHBENCHMARK_QUERY15_OL(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY15, CHBENCHMARK_QUERY15_OL(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY15, piece: %d", CHBENCHMARK_QUERY15_OL(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all stock shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY15, CHBENCHMARK_QUERY15_ST(i),
                  CHBENCHMARK_TB_STOCK, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY15, CHBENCHMARK_QUERY15_ST(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY15, CHBENCHMARK_QUERY15_ST(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY15, piece: %d", CHBENCHMARK_QUERY15_ST(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all supplier shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY15, CHBENCHMARK_QUERY15_SP(i),
                  CHBENCHMARK_TB_SUPPLIER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY15, CHBENCHMARK_QUERY15_SP(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY15, CHBENCHMARK_QUERY15_SP(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY15, piece: %d", CHBENCHMARK_QUERY15_SP(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE
}
}// namespace rococo
