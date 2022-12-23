#include "chopper.h"
#include "generator.h"
#include "piece.h"

namespace rococo {

void ChbenchmarkTxn::Query3Init(TxnRequest &req) {
    Query3Retry();
}

void ChbenchmarkTxn::Query3Retry() {
    for (int i = 0; i < Config::GetConfig()->GetNumPartition(); i++) {
        status_[CHBENCHMARK_QUERY3_CT(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY3_NO(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY3_OD(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY3_OL(i)] = DISPATCHABLE;
    }
    n_pieces_all_ = Config::GetConfig()->GetNumPartition();//one partition one warehouse
    n_pieces_dispatchable_ = Config::GetConfig()->GetNumPartition();
    n_pieces_dispatch_acked_ = 0;
    n_pieces_dispatched_ = 0;
    CheckReady();
}

void ChbenchmarkPiece::RegQuery3() {
    //read all customer shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY3, CHBENCHMARK_QUERY3_CT(i),
                  CHBENCHMARK_TB_CUSTOMER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY3, CHBENCHMARK_QUERY3_CT(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY3, CHBENCHMARK_QUERY3_CT(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY3, piece: %d", CHBENCHMARK_QUERY3_CT(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all new order shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY3, CHBENCHMARK_QUERY3_NO(i),
                  CHBENCHMARK_TB_NEW_ORDER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY3, CHBENCHMARK_QUERY3_NO(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY3, CHBENCHMARK_QUERY3_NO(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY3, piece: %d", CHBENCHMARK_QUERY3_NO(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all order shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY3, CHBENCHMARK_QUERY3_OD(i),
                  CHBENCHMARK_TB_ORDER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY3, CHBENCHMARK_QUERY3_OD(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY3, CHBENCHMARK_QUERY3_OD(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY3, piece: %d", CHBENCHMARK_QUERY3_OD(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all order line shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY3, CHBENCHMARK_QUERY3_OL(i),
                  CHBENCHMARK_TB_ORDER_LINE, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY3, CHBENCHMARK_QUERY3_OL(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY3, CHBENCHMARK_QUERY3_OL(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY3, piece: %d", CHBENCHMARK_QUERY3_OL(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE
}
}// namespace rococo
