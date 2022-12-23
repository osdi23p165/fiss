#include "chopper.h"
#include "generator.h"
#include "piece.h"

namespace rococo {

void ChbenchmarkTxn::Query13Init(TxnRequest &req) {
    Query13Retry();
}

void ChbenchmarkTxn::Query13Retry() {
    for (int i = 0; i < Config::GetConfig()->GetNumPartition(); i++) {
        status_[CHBENCHMARK_QUERY13_CT(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY13_OD(i)] = DISPATCHABLE;
    }
    n_pieces_all_ = Config::GetConfig()->GetNumPartition();//one partition one warehouse
    n_pieces_dispatchable_ = Config::GetConfig()->GetNumPartition();
    n_pieces_dispatch_acked_ = 0;
    n_pieces_dispatched_ = 0;
    CheckReady();
}

void ChbenchmarkPiece::RegQuery13() {
    //read all customer shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY13, CHBENCHMARK_QUERY13_CT(i),
                  CHBENCHMARK_TB_CUSTOMER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY13, CHBENCHMARK_QUERY13_CT(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY13, CHBENCHMARK_QUERY13_CT(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY13, piece: %d", CHBENCHMARK_QUERY13_CT(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all order shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY13, CHBENCHMARK_QUERY13_OD(i),
                  CHBENCHMARK_TB_ORDER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY13, CHBENCHMARK_QUERY13_OD(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY13, CHBENCHMARK_QUERY13_OD(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY13, piece: %d", CHBENCHMARK_QUERY13_OD(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE
}
}// namespace rococo
