#include "chopper.h"
#include "generator.h"
#include "piece.h"

namespace rococo {

void ChbenchmarkTxn::Query10Init(TxnRequest &req) {
    Query10Retry();
}

void ChbenchmarkTxn::Query10Retry() {
    for (int i = 0; i < Config::GetConfig()->GetNumPartition(); i++) {
        status_[CHBENCHMARK_QUERY10_CT(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY10_OD(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY10_OL(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY10_NT(i)] = DISPATCHABLE;
    }
    n_pieces_all_ = Config::GetConfig()->GetNumPartition();//one partition one warehouse
    n_pieces_dispatchable_ = Config::GetConfig()->GetNumPartition();
    n_pieces_dispatch_acked_ = 0;
    n_pieces_dispatched_ = 0;
    CheckReady();
}

void ChbenchmarkPiece::RegQuery10() {
    //read all customer shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY10, CHBENCHMARK_QUERY10_CT(i),
                  CHBENCHMARK_TB_CUSTOMER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY10, CHBENCHMARK_QUERY10_CT(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY10, CHBENCHMARK_QUERY10_CT(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY10, piece: %d", CHBENCHMARK_QUERY10_CT(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all order shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY10, CHBENCHMARK_QUERY10_OD(i),
                  CHBENCHMARK_TB_ORDER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY10, CHBENCHMARK_QUERY10_OD(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY10, CHBENCHMARK_QUERY10_OD(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY10, piece: %d", CHBENCHMARK_QUERY10_OD(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all order line shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY10, CHBENCHMARK_QUERY10_OL(i),
                  CHBENCHMARK_TB_ORDER_LINE, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY10, CHBENCHMARK_QUERY10_OL(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY10, CHBENCHMARK_QUERY10_OL(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY10, piece: %d", CHBENCHMARK_QUERY10_OL(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all nation shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY10, CHBENCHMARK_QUERY10_NT(i),
                  CHBENCHMARK_TB_NATION, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY10, CHBENCHMARK_QUERY10_NT(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY10, CHBENCHMARK_QUERY10_NT(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY10, piece: %d", CHBENCHMARK_QUERY10_NT(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE
}
}// namespace rococo
