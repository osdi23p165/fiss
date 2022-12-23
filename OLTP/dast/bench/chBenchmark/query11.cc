#include "chopper.h"
#include "generator.h"
#include "piece.h"

namespace rococo {

void ChbenchmarkTxn::Query11Init(TxnRequest &req) {
    Query11Retry();
}

void ChbenchmarkTxn::Query11Retry() {
    for (int i = 0; i < Config::GetConfig()->GetNumPartition(); i++) {
        status_[CHBENCHMARK_QUERY11_ST(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY11_SP(i)] = DISPATCHABLE;
        status_[CHBENCHMARK_QUERY11_NT(i)] = DISPATCHABLE;
    }
    n_pieces_all_ = Config::GetConfig()->GetNumPartition();//one partition one warehouse
    n_pieces_dispatchable_ = Config::GetConfig()->GetNumPartition();
    n_pieces_dispatch_acked_ = 0;
    n_pieces_dispatched_ = 0;
    CheckReady();
}

void ChbenchmarkPiece::RegQuery11() {
    //read all stock shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY11, CHBENCHMARK_QUERY11_ST(i),
                  CHBENCHMARK_TB_STOCK, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY11, CHBENCHMARK_QUERY11_ST(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY11, CHBENCHMARK_QUERY11_ST(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY11, piece: %d", CHBENCHMARK_QUERY11_ST(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all supplier shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY11, CHBENCHMARK_QUERY11_SP(i),
                  CHBENCHMARK_TB_SUPPLIER, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY11, CHBENCHMARK_QUERY11_SP(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY11, CHBENCHMARK_QUERY11_SP(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY11, piece: %d", CHBENCHMARK_QUERY11_SP(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE

    //read all nation shard
    for (int i = (0); i < (1000); i++) {
        // 1000 is a magical number?
        SHARD_PIE(CHBENCHMARK_QUERY11, CHBENCHMARK_QUERY11_NT(i),
                  CHBENCHMARK_TB_NATION, CHBENCHMARK_QUERY_VAR_W_ID(i))
        INPUT_PIE(CHBENCHMARK_QUERY11, CHBENCHMARK_QUERY11_NT(i))
    }
    BEGIN_LOOP_PIE(CHBENCHMARK_QUERY11, CHBENCHMARK_QUERY11_NT(0), 1000, DF_NO) {
        Log_debug("CHBENCHMARK_QUERY11, piece: %d", CHBENCHMARK_QUERY11_NT(I));
        *res = SUCCESS;
        return;
    } END_LOOP_PIE
}
}// namespace rococo
