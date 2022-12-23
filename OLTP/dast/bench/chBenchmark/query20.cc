#include "chopper.h"
#include "generator.h"
#include "piece.h"

namespace rococo {

void ChbenchmarkTxn::Query20Init(TxnRequest &req) {
    Query20Retry();
}

void ChbenchmarkTxn::Query20Retry() {
    for (int i = 0; i < Config::GetConfig()->GetNumPartition(); i++) {
        status_[CHBENCHMARK_QUERY20_SP(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY20_NT(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY20_ST(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY20_OL(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY20_IT(i)] = DISPATCHABLE;
    }
    n_pieces_all_ = Config::GetConfig()->GetNumPartition();//one partition one warehouse
    n_pieces_dispatchable_ = Config::GetConfig()->GetNumPartition();
    n_pieces_dispatch_acked_ = 0;
    n_pieces_dispatched_ = 0;
    CheckReady();
}

void ChbenchmarkPiece::RegQuery20() {
    //read all supplier shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY20, CHBENCHMARK_QUERY20_SP(i),
                  CHBENCHMARK_TB_SUPPLIER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY20, CHBENCHMARK_QUERY20_SP(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY20, CHBENCHMARK_QUERY20_SP(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY20, piece: %d", CHBENCHMARK_QUERY20_SP(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all nation shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY20, CHBENCHMARK_QUERY20_NT(i),
                  CHBENCHMARK_TB_NATION, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY20, CHBENCHMARK_QUERY20_NT(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY20, CHBENCHMARK_QUERY20_NT(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY20, piece: %d", CHBENCHMARK_QUERY20_NT(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all stock shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY20, CHBENCHMARK_QUERY20_ST(i),
                  CHBENCHMARK_TB_STOCK, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY20, CHBENCHMARK_QUERY20_ST(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY20, CHBENCHMARK_QUERY20_ST(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY20, piece: %d", CHBENCHMARK_QUERY20_ST(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all order line shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY20, CHBENCHMARK_QUERY20_OL(i),
                  CHBENCHMARK_TB_ORDER_LINE, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY20, CHBENCHMARK_QUERY20_OL(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY20, CHBENCHMARK_QUERY20_OL(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY20, piece: %d", CHBENCHMARK_QUERY20_OL(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all item shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY20, CHBENCHMARK_QUERY20_IT(i),
                  CHBENCHMARK_TB_ITEM, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY20, CHBENCHMARK_QUERY20_IT(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY20, CHBENCHMARK_QUERY20_IT(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY20, piece: %d", CHBENCHMARK_QUERY20_IT(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE
}
}// namespace rococo
