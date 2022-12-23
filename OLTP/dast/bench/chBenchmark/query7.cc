#include "chopper.h"
#include "generator.h"
#include "piece.h"

namespace rococo {

void ChbenchmarkTxn::Query7Init(TxnRequest &req) {
    Query7Retry();
}

void ChbenchmarkTxn::Query7Retry() {
    for (int i = 0; i < Config::GetConfig()->GetNumPartition(); i++) {
        status_[CHBENCHMARK_QUERY7_SP(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY7_ST(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY7_OL(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY7_OD(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY7_CT(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY7_NT(i)] = DISPATCHABLE;
    }
    n_pieces_all_ = Config::GetConfig()->GetNumPartition();//one partition one warehouse
    n_pieces_dispatchable_ = Config::GetConfig()->GetNumPartition();
    n_pieces_dispatch_acked_ = 0;
    n_pieces_dispatched_ = 0;
    CheckReady();
}

void ChbenchmarkPiece::RegQuery7() {
    //read all supplier shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY7, CHBENCHMARK_QUERY7_SP(i),
                  CHBENCHMARK_TB_SUPPLIER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY7, CHBENCHMARK_QUERY7_SP(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY7, CHBENCHMARK_QUERY7_SP(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY7, piece: %d", CHBENCHMARK_QUERY7_SP(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all stock shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY7, CHBENCHMARK_QUERY7_ST(i),
                  CHBENCHMARK_TB_STOCK, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY7, CHBENCHMARK_QUERY7_ST(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY7, CHBENCHMARK_QUERY7_ST(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY7, piece: %d", CHBENCHMARK_QUERY7_ST(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all order line shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY7, CHBENCHMARK_QUERY7_OL(i),
                  CHBENCHMARK_TB_ORDER_LINE, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY7, CHBENCHMARK_QUERY7_OL(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY7, CHBENCHMARK_QUERY7_OL(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY7, piece: %d", CHBENCHMARK_QUERY7_OL(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all order shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY7, CHBENCHMARK_QUERY7_OD(i),
                  CHBENCHMARK_TB_ORDER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY7, CHBENCHMARK_QUERY7_OD(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY7, CHBENCHMARK_QUERY7_OD(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY7, piece: %d", CHBENCHMARK_QUERY7_OD(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all customer shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY7, CHBENCHMARK_QUERY7_CT(i),
                  CHBENCHMARK_TB_CUSTOMER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY7, CHBENCHMARK_QUERY7_CT(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY7, CHBENCHMARK_QUERY7_CT(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY7, piece: %d", CHBENCHMARK_QUERY7_CT(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all nation shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY7, CHBENCHMARK_QUERY7_NT(i),
                  CHBENCHMARK_TB_NATION, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY7, CHBENCHMARK_QUERY7_NT(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY7, CHBENCHMARK_QUERY7_NT(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY7, piece: %d", CHBENCHMARK_QUERY7_NT(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE
}
}// namespace rococo
