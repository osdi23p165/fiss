#include "chopper.h"
#include "generator.h"
#include "piece.h"

namespace rococo {

void ChbenchmarkTxn::Query19Init(TxnRequest &req) {
    Query19Retry();
}

void ChbenchmarkTxn::Query19Retry() {
    for (int i = 0; i < Config::GetConfig()->GetNumPartition(); i++) {
        status_[CHBENCHMARK_QUERY19_OL(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY19_IT(i)] = DISPATCHABLE;
    }
    n_pieces_all_ = Config::GetConfig()->GetNumPartition();//one partition one warehouse
    n_pieces_dispatchable_ = Config::GetConfig()->GetNumPartition();
    n_pieces_dispatch_acked_ = 0;
    n_pieces_dispatched_ = 0;
    CheckReady();
}

void ChbenchmarkPiece::RegQuery19() {
    //read all order line shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY19, CHBENCHMARK_QUERY19_OL(i),
                  CHBENCHMARK_TB_ORDER_LINE, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY19, CHBENCHMARK_QUERY19_OL(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY19, CHBENCHMARK_QUERY19_OL(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY19, piece: %d", CHBENCHMARK_QUERY19_OL(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all item shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY19, CHBENCHMARK_QUERY19_IT(i),
                  CHBENCHMARK_TB_ITEM, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY19, CHBENCHMARK_QUERY19_IT(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY19, CHBENCHMARK_QUERY19_IT(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY19, piece: %d", CHBENCHMARK_QUERY19_IT(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE
}
}// namespace rococo
