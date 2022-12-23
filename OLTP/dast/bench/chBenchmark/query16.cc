#include "chopper.h"
#include "generator.h"
#include "piece.h"

namespace rococo {

void ChbenchmarkTxn::Query16Init(TxnRequest &req) {
    Query16Retry();
}

void ChbenchmarkTxn::Query16Retry() {
    for (int i = 0; i < Config::GetConfig()->GetNumPartition(); i++) {
        status_[CHBENCHMARK_QUERY16_ST(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY16_IT(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY16_SP(i)] = DISPATCHABLE;
    }
    n_pieces_all_ = Config::GetConfig()->GetNumPartition();//one partition one warehouse
    n_pieces_dispatchable_ = Config::GetConfig()->GetNumPartition();
    n_pieces_dispatch_acked_ = 0;
    n_pieces_dispatched_ = 0;
    CheckReady();
}

void ChbenchmarkPiece::RegQuery16() {
    //read all stock shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY16, CHBENCHMARK_QUERY16_ST(i),
                  CHBENCHMARK_TB_STOCK, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY16, CHBENCHMARK_QUERY16_ST(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY16, CHBENCHMARK_QUERY16_ST(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY16, piece: %d", CHBENCHMARK_QUERY16_ST(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all item shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY16, CHBENCHMARK_QUERY16_IT(i),
                  CHBENCHMARK_TB_ITEM, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY16, CHBENCHMARK_QUERY16_IT(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY16, CHBENCHMARK_QUERY16_IT(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY16, piece: %d", CHBENCHMARK_QUERY16_IT(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all supplier shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY16, CHBENCHMARK_QUERY16_SP(i),
                  CHBENCHMARK_TB_SUPPLIER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY16, CHBENCHMARK_QUERY16_SP(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY16, CHBENCHMARK_QUERY16_SP(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY16, piece: %d", CHBENCHMARK_QUERY16_SP(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE
}
}// namespace rococo
