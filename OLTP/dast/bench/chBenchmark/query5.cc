#include "chopper.h"
#include "generator.h"
#include "piece.h"

namespace rococo {

void ChbenchmarkTxn::Query5Init(TxnRequest &req) {
    Query5Retry();
}

void ChbenchmarkTxn::Query5Retry() {
    for (int i = 0; i < Config::GetConfig()->GetNumPartition(); i++) {
        status_[CHBENCHMARK_QUERY5_CT(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY5_OD(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY5_OL(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY5_ST(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY5_SP(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY5_NT(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY5_RG(i)] = DISPATCHABLE;
    }
    n_pieces_all_ = Config::GetConfig()->GetNumPartition();//one partition one warehouse
    n_pieces_dispatchable_ = Config::GetConfig()->GetNumPartition();
    n_pieces_dispatch_acked_ = 0;
    n_pieces_dispatched_ = 0;
    CheckReady();
}

void ChbenchmarkPiece::RegQuery5() {
    //read all customer shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_CT(i),
                  CHBENCHMARK_TB_CUSTOMER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_CT(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_CT(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY5, piece: %d", CHBENCHMARK_QUERY5_CT(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all order shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_OD(i),
                  CHBENCHMARK_TB_ORDER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_OD(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_OD(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY5, piece: %d", CHBENCHMARK_QUERY5_OD(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE


    //read all order line shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_OL(i),
                  CHBENCHMARK_TB_ORDER_LINE, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_OL(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_OL(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY5, piece: %d", CHBENCHMARK_QUERY5_OL(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all stock shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_ST(i),
                  CHBENCHMARK_TB_STOCK, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_ST(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_ST(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY5, piece: %d", CHBENCHMARK_QUERY5_ST(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all supplier shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_SP(i),
                  CHBENCHMARK_TB_SUPPLIER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_SP(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_SP(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY5, piece: %d", CHBENCHMARK_QUERY5_SP(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all nation shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_NT(i),
                  CHBENCHMARK_TB_NATION, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_NT(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_NT(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY5, piece: %d", CHBENCHMARK_QUERY5_NT(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all region shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_RG(i),
                  CHBENCHMARK_TB_REGION, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_RG(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY5, CHBENCHMARK_QUERY5_RG(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY5, piece: %d", CHBENCHMARK_QUERY5_RG(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE
}
}// namespace rococo
