#include "chopper.h"
#include "generator.h"
#include "piece.h"

namespace rococo {

void ChbenchmarkTxn::Query8Init(TxnRequest &req) {
    Query8Retry();
}

void ChbenchmarkTxn::Query8Retry() {
    for (int i = 0; i < Config::GetConfig()->GetNumPartition(); i++) {
        status_[CHBENCHMARK_QUERY8_IT(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY8_SP(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY8_ST(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY8_OL(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY8_OD(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY8_CT(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY8_RG(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY8_NT(i)] = DISPATCHABLE;
    }
    n_pieces_all_ = Config::GetConfig()->GetNumPartition();//one partition one warehouse
    n_pieces_dispatchable_ = Config::GetConfig()->GetNumPartition();
    n_pieces_dispatch_acked_ = 0;
    n_pieces_dispatched_ = 0;
    CheckReady();
}

void ChbenchmarkPiece::RegQuery8() {
    //read all item shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_IT(i),
                  CHBENCHMARK_TB_ITEM, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_IT(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_IT(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY8, piece: %d", CHBENCHMARK_QUERY8_IT(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all supplier shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_SP(i),
                  CHBENCHMARK_TB_SUPPLIER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_SP(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_SP(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY8, piece: %d", CHBENCHMARK_QUERY8_SP(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all stock shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_ST(i),
                  CHBENCHMARK_TB_STOCK, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_ST(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_ST(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY8, piece: %d", CHBENCHMARK_QUERY8_ST(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all order line shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_OL(i),
                  CHBENCHMARK_TB_ORDER_LINE, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_OL(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_OL(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY8, piece: %d", CHBENCHMARK_QUERY8_OL(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all order shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_OD(i),
                  CHBENCHMARK_TB_ORDER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_OD(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_OD(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY8, piece: %d", CHBENCHMARK_QUERY8_OD(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all customer shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_CT(i),
                  CHBENCHMARK_TB_CUSTOMER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_CT(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_CT(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY8, piece: %d", CHBENCHMARK_QUERY8_CT(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all region shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_RG(i),
                  CHBENCHMARK_TB_REGION, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_RG(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_RG(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY8, piece: %d", CHBENCHMARK_QUERY8_RG(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all nation shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_NT(i),
                  CHBENCHMARK_TB_NATION, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_NT(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY8, CHBENCHMARK_QUERY8_NT(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY8, piece: %d", CHBENCHMARK_QUERY8_NT(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE
}
}// namespace rococo
