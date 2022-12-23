#include "config.h"
#include "constants.h"
#include "sharding.h"
#include "txn_req_factory.h"

// for tpca benchmark
#include "bench/tpca/piece.h"
#include "bench/tpca/chopper.h"

// tpcc benchmark
#include "bench/tpcc/piece.h"
#include "bench/tpcc/chopper.h"

// tpcc dist partition benchmark
#include "bench/tpcc_dist/piece.h"
#include "bench/tpcc_dist/chopper.h"

// tpcc real dist partition benchmark
#include "bench/tpcc_real_dist/piece.h"
#include "bench/tpcc_real_dist/chopper.h"

// rw benchmark
#include "bench/rw_benchmark/piece.h"
#include "bench/rw_benchmark/chopper.h"

// micro bench
#include "bench/micro/piece.h"
#include "bench/micro/chopper.h"

//retwis bench
#include "bench/retwis/piece.h"
#include "bench/retwis/chopper.h"

//ch benchmark
#include "bench/chBenchmark/piece.h"
#include "bench/chBenchmark/chopper.h"

namespace rococo {

    TxnGenerator::TxnGenerator(Config *config)
            : txn_weight_(config->get_txn_weight()),
              txn_weights_(config->get_txn_weights()),
              sharding_(config->sharding_) {
        benchmark_ = Config::GetConfig()->get_benchmark();
        n_try_ = Config::GetConfig()->get_max_retry();
        single_server_ = Config::GetConfig()->get_single_server();

        std::map <std::string, uint64_t> table_num_rows;
        sharding_->get_number_rows(table_num_rows);

        if (Config::GetConfig()->dist_ == "fixed") {
            single_server_ = Config::SS_PROCESS_SINGLE;
        }

        switch (benchmark_) {
            case MICRO_BENCH:
                micro_bench_para_.n_table_a_ = table_num_rows[std::string(MICRO_BENCH_TABLE_A)];
                micro_bench_para_.n_table_b_ = table_num_rows[std::string(MICRO_BENCH_TABLE_B)];
                micro_bench_para_.n_table_c_ = table_num_rows[std::string(MICRO_BENCH_TABLE_C)];
                micro_bench_para_.n_table_d_ = table_num_rows[std::string(MICRO_BENCH_TABLE_D)];
                break;
            case TPCA:
            case TPCC:
            case CH_BENCHMARK:
            case TPCC_DIST_PART:
            case TPCC_REAL_DIST_PART: {
                break;
            }
            case RW_BENCHMARK:
                rw_benchmark_para_.n_table_ = table_num_rows[std::string(RW_BENCHMARK_TABLE)];
                fix_id_ = (Config::GetConfig()->dist_ == "fixed") ?
                          RandomGenerator::rand(0, rw_benchmark_para_.n_table_) :
                          -1;
                break;
            case RETWIS:
                retwis_para_.n_table_ = table_num_rows[std::string(RETWIS_TB)];
                fix_id_ = (Config::GetConfig()->dist_ == "fixed") ?
                          RandomGenerator::rand(0, retwis_para_.n_table_) :
                          -1;
                break;
            default:
                Log_fatal("benchmark not implemented");
                verify(0);
        }
    }


    void TxnGenerator::get_micro_bench_read_req(TxnRequest *req, uint32_t cid) const {
        req->txn_type_ = MICRO_BENCH_R;
        req->input_[0] = Value((i32) RandomGenerator::rand(0, micro_bench_para_.n_table_a_ - 1));
        req->input_[1] = Value((i32) RandomGenerator::rand(0, micro_bench_para_.n_table_b_ - 1));
        req->input_[2] = Value((i32) RandomGenerator::rand(0, micro_bench_para_.n_table_c_ - 1));
        req->input_[3] = Value((i32) RandomGenerator::rand(0, micro_bench_para_.n_table_d_ - 1));
    }

    void TxnGenerator::get_micro_bench_write_req(TxnRequest *req, uint32_t cid) const {
        req->txn_type_ = MICRO_BENCH_W;
        req->input_[0] = Value((i32) RandomGenerator::rand(0, micro_bench_para_.n_table_a_ - 1));
        req->input_[1] = Value((i32) RandomGenerator::rand(0, micro_bench_para_.n_table_b_ - 1));
        req->input_[2] = Value((i32) RandomGenerator::rand(0, micro_bench_para_.n_table_c_ - 1));
        req->input_[3] = Value((i32) RandomGenerator::rand(0, micro_bench_para_.n_table_d_ - 1));
        req->input_[4] = Value((i64) RandomGenerator::rand(0, 1000));
        req->input_[5] = Value((i64) RandomGenerator::rand(0, 1000));
        req->input_[6] = Value((i64) RandomGenerator::rand(0, 1000));
        req->input_[7] = Value((i64) RandomGenerator::rand(0, 1000));
    }

    void TxnGenerator::get_micro_bench_txn_req(
            TxnRequest *req, uint32_t cid) const {
        req->n_try_ = n_try_;
        if (txn_weight_.size() != 2)
            get_micro_bench_read_req(req, cid);
        else
            switch (RandomGenerator::weighted_select(txn_weight_)) {
                case 0: // read
                    get_micro_bench_read_req(req, cid);
                    break;
                case 1: // write
                    get_micro_bench_write_req(req, cid);
                    break;
            }

    }

    void TxnGenerator::GetTxnReq(TxnRequest *req, uint32_t cid) {
        switch (benchmark_) {
            case RW_BENCHMARK:
            case RETWIS:
            case TPCA:
            case TPCC:
            case CH_BENCHMARK:
            case TPCC_DIST_PART:
            case TPCC_REAL_DIST_PART:
                // should be called in sub-classes.
                verify(0);
                break;
            case MICRO_BENCH:
                get_micro_bench_txn_req(req, cid);
                break;
            default:
                Log_fatal("benchmark not implemented");
                verify(0);
        }
    }

    void TxnGenerator::get_txn_types(
            std::map <int32_t, std::string> &txn_types) {
        txn_types.clear();
        switch (benchmark_) {
            case TPCA:
                txn_types[TPCA_PAYMENT] = std::string(TPCA_PAYMENT_NAME);
                break;
            case TPCC:
            case TPCC_DIST_PART:
            case TPCC_REAL_DIST_PART:
                txn_types[TPCC_NEW_ORDER] = std::string(TPCC_NEW_ORDER_NAME);
                txn_types[TPCC_PAYMENT] = std::string(TPCC_PAYMENT_NAME);
                txn_types[TPCC_STOCK_LEVEL] = std::string(TPCC_STOCK_LEVEL_NAME);
                txn_types[TPCC_DELIVERY] = std::string(TPCC_DELIVERY_NAME);
                txn_types[TPCC_ORDER_STATUS] = std::string(TPCC_ORDER_STATUS_NAME);
                break;
            case CH_BENCHMARK:
                txn_types[CHBENCHMARK_NEW_ORDER] = std::string(CHBENCHMARK_NEW_ORDER_NAME);
                txn_types[CHBENCHMARK_PAYMENT] = std::string(CHBENCHMARK_PAYMENT_NAME);
                txn_types[CHBENCHMARK_STOCK_LEVEL] = std::string(CHBENCHMARK_STOCK_LEVEL_NAME);
                txn_types[CHBENCHMARK_DELIVERY] = std::string(CHBENCHMARK_DELIVERY_NAME);
                txn_types[CHBENCHMARK_ORDER_STATUS] = std::string(CHBENCHMARK_ORDER_STATUS_NAME);
                txn_types[CHBENCHMARK_QUERY1] = std::string(CHBENCHMARK_QUERY1_NAME);
                txn_types[CHBENCHMARK_QUERY2] = std::string(CHBENCHMARK_QUERY2_NAME);
                txn_types[CHBENCHMARK_QUERY3] = std::string(CHBENCHMARK_QUERY3_NAME);
                txn_types[CHBENCHMARK_QUERY4] = std::string(CHBENCHMARK_QUERY4_NAME);
                txn_types[CHBENCHMARK_QUERY5] = std::string(CHBENCHMARK_QUERY5_NAME);
                txn_types[CHBENCHMARK_QUERY6] = std::string(CHBENCHMARK_QUERY6_NAME);
                txn_types[CHBENCHMARK_QUERY7] = std::string(CHBENCHMARK_QUERY7_NAME);
                txn_types[CHBENCHMARK_QUERY8] = std::string(CHBENCHMARK_QUERY8_NAME);
                txn_types[CHBENCHMARK_QUERY9] = std::string(CHBENCHMARK_QUERY9_NAME);
                txn_types[CHBENCHMARK_QUERY10] = std::string(CHBENCHMARK_QUERY10_NAME);
                txn_types[CHBENCHMARK_QUERY11] = std::string(CHBENCHMARK_QUERY11_NAME);
                txn_types[CHBENCHMARK_QUERY12] = std::string(CHBENCHMARK_QUERY12_NAME);
                txn_types[CHBENCHMARK_QUERY13] = std::string(CHBENCHMARK_QUERY13_NAME);
                txn_types[CHBENCHMARK_QUERY14] = std::string(CHBENCHMARK_QUERY14_NAME);
                txn_types[CHBENCHMARK_QUERY15] = std::string(CHBENCHMARK_QUERY15_NAME);
                txn_types[CHBENCHMARK_QUERY16] = std::string(CHBENCHMARK_QUERY16_NAME);
                txn_types[CHBENCHMARK_QUERY17] = std::string(CHBENCHMARK_QUERY17_NAME);
                txn_types[CHBENCHMARK_QUERY18] = std::string(CHBENCHMARK_QUERY18_NAME);
                txn_types[CHBENCHMARK_QUERY19] = std::string(CHBENCHMARK_QUERY19_NAME);
                txn_types[CHBENCHMARK_QUERY20] = std::string(CHBENCHMARK_QUERY20_NAME);
                txn_types[CHBENCHMARK_QUERY21] = std::string(CHBENCHMARK_QUERY21_NAME);
                txn_types[CHBENCHMARK_QUERY22] = std::string(CHBENCHMARK_QUERY22_NAME);
                break;
            case RW_BENCHMARK:
                txn_types[RW_BENCHMARK_W_TXN] = std::string(RW_BENCHMARK_W_TXN_NAME);
                txn_types[RW_BENCHMARK_R_TXN] = std::string(RW_BENCHMARK_R_TXN_NAME);
                break;
            case MICRO_BENCH:
                txn_types[MICRO_BENCH_R] = std::string(MICRO_BENCH_R_NAME);
                txn_types[MICRO_BENCH_W] = std::string(MICRO_BENCH_W_NAME);
                break;
            case RETWIS:
                txn_types[RETWIS_ADD_USERS] = std::string(RETWIS_ADD_USERS_NAME);
                txn_types[RETWIS_FOLLOW] = std::string(RETWIS_FOLLOW_NAME);
                txn_types[RETWIS_POST_TWEET] = std::string(RETWIS_POST_TWEET_NAME);
                txn_types[RETWIS_GET_TIMELINE] = std::string(RETWIS_GET_TIMELINE_NAME);
                break;
            default:
                Log_fatal("benchmark not implemented");
                verify(0);
        }
    }

    TxnGenerator::~TxnGenerator() {
    }

} // namespace rcc
