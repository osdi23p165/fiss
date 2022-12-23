#include "chopper.h"
#include "generator.h"
#include "piece.h"

namespace rococo {

void ChbenchmarkTxn::Query2Init(TxnRequest &req) {
    Query2Retry();
}

void ChbenchmarkTxn::Query2Retry() {
    for (int i = 0; i < Config::GetConfig()->GetNumPartition(); i++) {
        status_[CHBENCHMARK_QUERY2_ST(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY2_SP(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY2_IT(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY2_NT(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY2_RG(i)] = DISPATCHABLE;
    }
    n_pieces_all_ = Config::GetConfig()->GetNumPartition();//one partition one warehouse
    n_pieces_dispatchable_ = Config::GetConfig()->GetNumPartition();
    n_pieces_dispatch_acked_ = 0;
    n_pieces_dispatched_ = 0;
    CheckReady();
}

void ChbenchmarkPiece::RegQuery2() {
    //read all stock shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY2, CHBENCHMARK_QUERY2_ST(i),
                  CHBENCHMARK_TB_STOCK, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY2, CHBENCHMARK_QUERY2_ST(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY2, CHBENCHMARK_QUERY2_ST(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY2, piece: %d", CHBENCHMARK_QUERY2_ST(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all supplier shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY2, CHBENCHMARK_QUERY2_SP(i),
                  CHBENCHMARK_TB_SUPPLIER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY2, CHBENCHMARK_QUERY2_SP(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY2, CHBENCHMARK_QUERY2_SP(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY2, piece: %d", CHBENCHMARK_QUERY2_SP(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all item shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY2, CHBENCHMARK_QUERY2_IT(i),
                  CHBENCHMARK_TB_ITEM, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY2, CHBENCHMARK_QUERY2_IT(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY2, CHBENCHMARK_QUERY2_IT(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY2, piece: %d", CHBENCHMARK_QUERY2_IT(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all nation shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY2, CHBENCHMARK_QUERY2_NT(i),
                  CHBENCHMARK_TB_NATION, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY2, CHBENCHMARK_QUERY2_NT(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY2, CHBENCHMARK_QUERY2_NT(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY2, piece: %d", CHBENCHMARK_QUERY2_NT(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all region shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY2, CHBENCHMARK_QUERY2_RG(i),
                  CHBENCHMARK_TB_REGION, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY2, CHBENCHMARK_QUERY2_RG(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY2, CHBENCHMARK_QUERY2_RG(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY2, piece: %d", CHBENCHMARK_QUERY2_RG(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE
}
}// namespace rococo
