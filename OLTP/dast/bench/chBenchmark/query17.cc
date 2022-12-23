#include "chopper.h"
#include "generator.h"
#include "piece.h"

namespace rococo {

void ChbenchmarkTxn::Query17Init(TxnRequest &req) {
    Query17Retry();
}

void ChbenchmarkTxn::Query17Retry() {
    for (int i = 0; i < Config::GetConfig()->GetNumPartition(); i++) {
        status_[CHBENCHMARK_QUERY17_IT(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY17_OL(i)] = DISPATCHABLE;
    }
    n_pieces_all_ = Config::GetConfig()->GetNumPartition();//one partition one warehouse
    n_pieces_dispatchable_ = Config::GetConfig()->GetNumPartition();
    n_pieces_dispatch_acked_ = 0;
    n_pieces_dispatched_ = 0;
    CheckReady();
}

void ChbenchmarkPiece::RegQuery17() {
    //read all item shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY17, CHBENCHMARK_QUERY17_IT(i),
                  CHBENCHMARK_TB_ITEM, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY17, CHBENCHMARK_QUERY17_IT(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY17, CHBENCHMARK_QUERY17_IT(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY17, piece: %d", CHBENCHMARK_QUERY17_IT(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all order line shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY17, CHBENCHMARK_QUERY17_OL(i),
                  CHBENCHMARK_TB_ORDER_LINE, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY17, CHBENCHMARK_QUERY17_OL(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY17, CHBENCHMARK_QUERY17_OL(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY17, piece: %d", CHBENCHMARK_QUERY17_OL(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE
}
}// namespace rococo
