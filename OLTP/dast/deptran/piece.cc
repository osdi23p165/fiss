#include "piece.h"
#include "bench/rw_benchmark/piece.h"
#include "bench/micro/piece.h"
#include "bench/tpca/piece.h"
#include "bench/tpcc/piece.h"
#include "bench/chBenchmark/piece.h"
#include "bench/tpcc_dist/piece.h"
#include "bench/tpcc_real_dist/piece.h"
#include "bench/retwis/piece.h"

namespace rococo {

    Piece *Piece::get_piece(int benchmark) {
        switch (benchmark) {
            case TPCA:
                return new TpcaPiece();
            case CH_BENCHMARK:
                return new ChbenchmarkPiece();
            case TPCC:
                return new TpccPiece();
            case TPCC_DIST_PART:
                return new TpccDistPiece();
            case TPCC_REAL_DIST_PART:
                return new TpccRealDistPiece();
            case RW_BENCHMARK:
                return new RWPiece();
            case MICRO_BENCH:
                return new MicroBenchPiece();
            case RETWIS:
                return new RetwisPiece();
            default:
                verify(0);
                return NULL;
        }
    }

} // namespace rococo
