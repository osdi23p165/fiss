#include "chopper.h"
#include "generator.h"
#include "piece.h"

namespace rococo {

void ChbenchmarkTxn::Query6Init(TxnRequest &req) {
    Query6Retry();
}

void ChbenchmarkTxn::Query6Retry() {
    for (int i = 0; i < Config::GetConfig()->GetNumPartition(); i++) {
        status_[CHBENCHMARK_QUERY6_OL(i)] = DISPATCHABLE;
    }
    n_pieces_all_ = Config::GetConfig()->GetNumPartition();//one partition one warehouse
    n_pieces_dispatchable_ = Config::GetConfig()->GetNumPartition();
    n_pieces_dispatch_acked_ = 0;
    n_pieces_dispatched_ = 0;
    CheckReady();
}

void ChbenchmarkPiece::RegQuery6() {
    //read all order line shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY6, CHBENCHMARK_QUERY6_OL(i),
                  CHBENCHMARK_TB_ORDER_LINE, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY6, CHBENCHMARK_QUERY6_OL(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY6, CHBENCHMARK_QUERY6_OL(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY6, piece: %d", CHBENCHMARK_QUERY6_OL(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE
}
}// namespace rococo
