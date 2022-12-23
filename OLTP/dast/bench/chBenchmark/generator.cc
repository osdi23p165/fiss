
#include "generator.h"
#include "deptran/txn_chopper.h"
#include "piece.h"

namespace rococo {

    ChbenchmarkTxnGenerator::ChbenchmarkTxnGenerator(Config *config, parid_t partition_id) : TxnGenerator(config) {
        std::map <std::string, uint64_t> table_num_rows;
        sharding_->get_number_rows(table_num_rows);
        this->partition_id_ = partition_id;

        std::vector<unsigned int> partitions;
        sharding_->GetTablePartitions(CHBENCHMARK_TB_WAREHOUSE, partitions);
        uint64_t tb_w_rows = table_num_rows[std::string(CHBENCHMARK_TB_WAREHOUSE)];
        chbenchmark_para_.n_w_id_ = (int) tb_w_rows * partitions.size();

        chbenchmark_para_.const_home_w_id_ = RandomGenerator::rand(0, chbenchmark_para_.n_w_id_ - 1);
        uint64_t tb_d_rows = table_num_rows[std::string(CHBENCHMARK_TB_DISTRICT)];
        chbenchmark_para_.n_d_id_ = (int) tb_d_rows / tb_w_rows;
        uint64_t tb_c_rows = table_num_rows[std::string(CHBENCHMARK_TB_CUSTOMER)];
        uint64_t tb_r_rows = table_num_rows[std::string(CHBENCHMARK_TB_REGION)];
        chbenchmark_para_.n_r_id_ = (int) tb_r_rows;
        chbenchmark_para_.n_c_id_ = (int) tb_c_rows / tb_d_rows;

        chbenchmark_para_.n_i_id_ = (int) table_num_rows[std::string(CHBENCHMARK_TB_ITEM)];
        chbenchmark_para_.delivery_d_id_ = RandomGenerator::rand(0, chbenchmark_para_.n_d_id_ - 1);
        switch (single_server_) {
            case Config::SS_DISABLED:
                fix_id_ = -1;
                break;
            case Config::SS_THREAD_SINGLE:
            case Config::SS_PROCESS_SINGLE: {
                fix_id_ = Config::GetConfig()->get_client_id() % chbenchmark_para_.n_d_id_;
                chbenchmark_para_.const_home_w_id_ =
                        Config::GetConfig()->get_client_id() / chbenchmark_para_.n_d_id_;
                break;
            }
            default:
                verify(0);
        }
    }


    void ChbenchmarkTxnGenerator::GetNewOrderTxnReq(TxnRequest *req,
                                                    uint32_t cid) const {
        req->is_local_txn_ = true;
        req->isQuery = false;
        req->txn_type_ = CHBENCHMARK_NEW_ORDER;
        //int home_w_id = RandomGenerator::rand(0, chbenchmark_para_.n_w_id_ - 1);
        int home_w_id = cid % chbenchmark_para_.n_w_id_;
        Value w_id((i32) home_w_id);
        req->home_region_id = home_w_id;
        //Value d_id((i32)RandomGenerator::rand(0, chbenchmark_para_.n_d_id_ - 1));
        Value d_id((i32)(cid / chbenchmark_para_.n_w_id_) % chbenchmark_para_.n_d_id_);
        Value c_id((i32) RandomGenerator::nu_rand(1022, 1, chbenchmark_para_.n_c_id_ - 1));
        int ol_cnt = RandomGenerator::rand(6, 15);
        //  int ol_cnt = 0;
        req->touchColsCnt = (int32_t)ol_cnt;

        rrr::i32 i_id_buf[ol_cnt];

        req->input_[CHBENCHMARK_VAR_W_ID] = w_id;
        req->input_[CHBENCHMARK_VAR_D_ID] = d_id;
        req->input_[CHBENCHMARK_VAR_C_ID] = c_id;
        req->input_[CHBENCHMARK_VAR_OL_CNT] = Value((i32) ol_cnt);
        req->input_[CHBENCHMARK_VAR_O_CARRIER_ID] = Value((int32_t) 0);
        bool all_local = true;
        for (int i = 0; i < ol_cnt; i++) {
            //req->input_[4 + 3 * i] = Value((i32)RandomGenerator::nu_rand(8191, 0, chbenchmark_para_.n_i_id_ - 1)); XXX nurand is the standard
            rrr::i32 tmp_i_id;
            while (true) {
                tmp_i_id = (i32) RandomGenerator::rand(0, chbenchmark_para_.n_i_id_ - 1 - i);
                if ((int) tmp_i_id % chbenchmark_para_.n_w_id_ == home_w_id) {
                    break;
                }
            }
            //int pre_n_less = 0, n_less = 0;
            //while (true) {
            //  n_less = 0;
            //  for (int j = 0; j < i; j++)
            //    if (i_id_buf[j] <= tmp_i_id)
            //      n_less++;
            //  if (n_less == pre_n_less)
            //    break;
            //  tmp_i_id += (n_less - pre_n_less);
            //  pre_n_less = n_less;
            //}

            i_id_buf[i] = tmp_i_id;
            req->input_[CHBENCHMARK_VAR_I_ID(i)] = Value(tmp_i_id);
            req->input_[CHBENCHMARK_VAR_OL_NUMBER(i)] = i;
            req->input_[CHBENCHMARK_VAR_OL_DELIVER_D(i)] = std::string();

            //auto config = Config::GetConfig();
            //int new_order_remote_rate = config->chbenchmark_payment_crt_rate_;
            //Log_info("new_order_remote_rate %d", new_order_remote_rate);
            if (chbenchmark_para_.n_w_id_ > 1 &&             // warehouse more than one, can do remote
                RandomGenerator::percentage_true(1)) {//XXX 1% REMOTE_RATIO
                int remote_w_id = RandomGenerator::rand(0, chbenchmark_para_.n_w_id_ - 2);
                remote_w_id = remote_w_id >= home_w_id ? remote_w_id + 1 : remote_w_id;
                if (!partitions_in_same_region((parid_t) remote_w_id, (parid_t) home_w_id)) {
                    req->is_local_txn_ = false;
                }

                req->input_[CHBENCHMARK_VAR_S_W_ID(i)] = Value((i32) remote_w_id);
                req->input_[CHBENCHMARK_VAR_S_REMOTE_CNT(i)] = Value((i32) 1);
                //      verify(req->input_[CHBENCHMARK_VAR_S_W_ID(i)].get_i32() < 3);
                all_local = false;
            } else {
                req->input_[CHBENCHMARK_VAR_S_W_ID(i)] = req->input_[CHBENCHMARK_VAR_W_ID];
                req->input_[CHBENCHMARK_VAR_S_REMOTE_CNT(i)] = Value((i32) 0);
                //      verify(req->input_[CHBENCHMARK_VAR_S_W_ID(i)].get_i32() < 3);
            }
            req->input_[CHBENCHMARK_VAR_OL_QUANTITY(i)] = Value((i32) RandomGenerator::rand(0, 10));
        }
        req->input_[CHBENCHMARK_VAR_O_ALL_LOCAL] = all_local ? Value((i32) 1) : Value((i32) 0);
    }


    void ChbenchmarkTxnGenerator::get_chbenchmark_payment_txn_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = true;
        req->isQuery = false;
        req->txn_type_ = CHBENCHMARK_PAYMENT;
        req->touchColsCnt = (int32_t)1;
        //DAST's Edge semantics
        int home_w_id = (int) cid % chbenchmark_para_.n_w_id_;
        req->home_region_id = home_w_id;
        Value c_w_id, c_d_id;
        Value w_id((i32) home_w_id);
        Value d_id((i32)(cid / chbenchmark_para_.n_w_id_) % chbenchmark_para_.n_d_id_);

        bool by_last_name = false;
        if (RandomGenerator::percentage_true(40)) {
            //This will cause cross-region dependencies.
            by_last_name = true;
            string cLast = "";
            ChbenchmarkTxnGenerator::randomCLast(cLast);
            req->input_[CHBENCHMARK_VAR_C_LAST] =
                    Value(cLast);
        } else {
            req->input_[CHBENCHMARK_VAR_C_ID] =
                    Value((i32) RandomGenerator::nu_rand(1022, 1, chbenchmark_para_.n_c_id_ - 1));
        }
        auto config = Config::GetConfig();
        int crt_ratio = config->chbenchmark_payment_crt_rate_;

        //NO need to work for N_W_ID = 1;
        verify(chbenchmark_para_.n_w_id_ > 1);

        Log_info("crt ratio %d", crt_ratio);

        if (crt_ratio >= 0) {
            // Testing different ratios of CRTs

            if (RandomGenerator::percentage_true(crt_ratio)) {
                // A CRT
                // Random select a shard that is not in my region.
                int c_w_id_int;
                do {
                    c_w_id_int = RandomGenerator::rand(0, chbenchmark_para_.n_w_id_ - 2);
                    c_w_id_int = c_w_id_int >= home_w_id ? c_w_id_int + 1 : c_w_id_int;
                    req->is_local_txn_ = partitions_in_same_region((parid_t) c_w_id_int, (parid_t) home_w_id);
                    Log_info("[CRT ratio test] c_w_id_int = %d, home_id = %d, is_local = %d, by_last_name = %d",
                             c_w_id_int, home_w_id,
                             req->is_local_txn_, by_last_name);
                } while (req->is_local_txn_);//this should be a distributed txn

                c_w_id = Value((i32) c_w_id_int);
                c_d_id = Value((i32) RandomGenerator::rand(0, chbenchmark_para_.n_d_id_ - 1));
            } else {
                // An IRT.
                // By default 15% will go to another shard within my region.
                // 85% will be single-shard transaction.
                req->is_local_txn_ = true;

                if (RandomGenerator::percentage_true(15)) {
                    int region_id = home_w_id / Config::GetConfig()->n_shard_per_region_;
                    int c_w_id_int = region_id * Config::GetConfig()->n_shard_per_region_ +
                                     RandomGenerator::rand(0, Config::GetConfig()->n_shard_per_region_ - 2);
                    c_w_id_int = c_w_id_int >= home_w_id ? c_w_id_int + 1 : c_w_id_int;
                    Log_info("[CRT ratio test] c_w_id_int = %d, home_id = %d, is_local = %d, by_last_name = %d",
                             c_w_id_int, home_w_id,
                             req->is_local_txn_, by_last_name);
                    c_w_id = Value((i32) c_w_id_int);
                    c_d_id = Value((i32) RandomGenerator::rand(0, chbenchmark_para_.n_d_id_ - 1));
                } else {
                    Log_info("[CRT ratio test], single-shard payment, home_id = %d", w_id.get_i32());
                    c_w_id = w_id;
                    c_d_id = d_id;
                }
            }
        } else if (crt_ratio == -1) {
            //Normal test, not setting specific CRT ratios

            if (RandomGenerator::percentage_true(15)) {
                //with 15\% rate, randomly select a remote shard (warehouse) from all warehouses.
                int c_w_id_int = RandomGenerator::rand(0, chbenchmark_para_.n_w_id_ - 2);
                c_w_id_int = c_w_id_int >= home_w_id ? c_w_id_int + 1 : c_w_id_int;
                req->is_local_txn_ = partitions_in_same_region((parid_t) c_w_id_int, (parid_t) home_w_id);
                Log_info("[Not in CRT ratio test] c_w_id_int = %d, home_id = %d, is_local = %d, by_last_name = %d",
                         c_w_id_int, home_w_id,
                         req->is_local_txn_, by_last_name);
                c_w_id = Value((i32) c_w_id_int);
                c_d_id = Value((i32) RandomGenerator::rand(0, chbenchmark_para_.n_d_id_ - 1));
            } else {
                //with 85\% rate, access the same shard (warehouse) as me.
                Log_info("[Not in CRT ratio test], single-shard payment, home_id = %d", home_w_id);
                c_w_id = w_id;
                c_d_id = d_id;
            }
        } else {
            Log_error(
                    "CRT ratio should be set a non-negative number for testing different CRT ratios, and \"-1\" for other tests");
            verify(0);
        }
        Value h_amount(RandomGenerator::rand_double(1.00, 5000.00));
        //  req->input_.resize(7);
        req->input_[CHBENCHMARK_VAR_W_ID] = w_id;
        req->input_[CHBENCHMARK_VAR_D_ID] = d_id;
        req->input_[CHBENCHMARK_VAR_C_W_ID] = c_w_id;
        req->input_[CHBENCHMARK_VAR_C_D_ID] = c_d_id;
        req->input_[CHBENCHMARK_VAR_H_AMOUNT] = h_amount;
        rrr::i32 tmp_h_key = home_w_id;
        req->input_[CHBENCHMARK_VAR_H_KEY] = Value(tmp_h_key);// h_key
        //  req->input_[CHBENCHMARK_VAR_W_NAME] = Value();
        //  req->input_[CHBENCHMARK_VAR_D_NAME] = Value();
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_stock_level_txn_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = true;
        req->isQuery = false;
        req->txn_type_ = CHBENCHMARK_STOCK_LEVEL;
        req->touchColsCnt = (int32_t)1;
        req->input_[CHBENCHMARK_VAR_W_ID] = Value((i32)(cid % chbenchmark_para_.n_w_id_));
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        req->input_[CHBENCHMARK_VAR_D_ID] = Value((i32)(cid / chbenchmark_para_.n_w_id_) % chbenchmark_para_.n_d_id_);
        req->input_[CHBENCHMARK_VAR_THRESHOLD] = Value((i32) RandomGenerator::rand(10, 20));
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_delivery_txn_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = true;
        req->isQuery = false;
        req->txn_type_ = CHBENCHMARK_DELIVERY;
        req->touchColsCnt = (int32_t)1;
        req->input_[CHBENCHMARK_VAR_W_ID] = Value((i32)(cid % chbenchmark_para_.n_w_id_));
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        req->input_[CHBENCHMARK_VAR_O_CARRIER_ID] = Value((i32) RandomGenerator::rand(1, 10));
        req->input_[CHBENCHMARK_VAR_D_ID] = Value((i32)(cid / chbenchmark_para_.n_w_id_) % chbenchmark_para_.n_d_id_);
    }


    void ChbenchmarkTxnGenerator::get_chbenchmark_order_status_txn_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = true;
        req->isQuery = false;
        req->txn_type_ = CHBENCHMARK_ORDER_STATUS;
        req->touchColsCnt = (int32_t)1;
        if (RandomGenerator::percentage_true(60)) {//XXX 60% by c_last
            string cLast = "";
            ChbenchmarkTxnGenerator::randomCLast(cLast);
            req->input_[CHBENCHMARK_VAR_C_LAST] =
                    Value(cLast);
        } else {
            req->input_[CHBENCHMARK_VAR_C_ID] =
                    Value((i32) RandomGenerator::nu_rand(1022, 1, chbenchmark_para_.n_c_id_ - 1));
        }
        req->input_[CHBENCHMARK_VAR_W_ID] = Value((i32)(cid % chbenchmark_para_.n_w_id_));
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        req->input_[CHBENCHMARK_VAR_D_ID] = Value((i32)((cid / chbenchmark_para_.n_w_id_) % chbenchmark_para_.n_d_id_));
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query1_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY1;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query2_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY2;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query3_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY3;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query4_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY4;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query5_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY5;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query6_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY6;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query7_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY7;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query8_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY8;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query9_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY9;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query10_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY10;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query11_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY11;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query12_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY12;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query13_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY13;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query14_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY14;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query15_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY15;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query16_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY16;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query17_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY17;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query18_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY18;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query19_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY19;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query20_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY20;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query21_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY21;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::get_chbenchmark_Query22_req(
            TxnRequest *req, uint32_t cid) const {
        req->is_local_txn_ = false;
        req->isQuery = true;
        req->txn_type_ = CHBENCHMARK_QUERY22;
        req->touchColsCnt = (int32_t)1;
        req->home_region_id = (i16)(cid % chbenchmark_para_.n_w_id_);
        for (int i = 0; i < chbenchmark_para_.n_w_id_; i++) {
            req->input_[CHBENCHMARK_QUERY_VAR_W_ID(i)] = Value((i32) i);
        }
    }

    void ChbenchmarkTxnGenerator::GetTxnReq(TxnRequest *req, uint32_t cid) {
        req->n_try_ = n_try_;
        if (txn_weight_.size() != 27) {
            verify(0);
            GetNewOrderTxnReq(req, cid);
        } else {
            switch (RandomGenerator::weighted_select(txn_weight_)) {
                case 0:
                    GetNewOrderTxnReq(req, cid);
                    break;
                case 1:
                    get_chbenchmark_payment_txn_req(req, cid);
                    break;
                case 2:
                    get_chbenchmark_order_status_txn_req(req, cid);
                    break;
                case 3:
                    get_chbenchmark_delivery_txn_req(req, cid);
                    break;
                case 4:
                    get_chbenchmark_stock_level_txn_req(req, cid);
                    break;
                case 5:
                    get_chbenchmark_Query1_req(req, cid);
                    break;
                case 6:
                    get_chbenchmark_Query2_req(req, cid);
                    break;
                case 7:
                    get_chbenchmark_Query3_req(req, cid);
                    break;
                case 8:
                    get_chbenchmark_Query4_req(req, cid);
                    break;
                case 9:
                    get_chbenchmark_Query5_req(req, cid);
                    break;
                case 10:
                    get_chbenchmark_Query6_req(req, cid);
                    break;
                case 11:
                    get_chbenchmark_Query7_req(req, cid);
                    break;
                case 12:
                    get_chbenchmark_Query8_req(req, cid);
                    break;
                case 13:
                    get_chbenchmark_Query9_req(req, cid);
                    break;
                case 14:
                    get_chbenchmark_Query10_req(req, cid);
                    break;
                case 15:
                    get_chbenchmark_Query11_req(req, cid);
                    break;
                case 16:
                    get_chbenchmark_Query12_req(req, cid);
                    break;
                case 17:
                    get_chbenchmark_Query13_req(req, cid);
                    break;
                case 18:
                    get_chbenchmark_Query14_req(req, cid);
                    break;
                case 19:
                    get_chbenchmark_Query15_req(req, cid);
                    break;
                case 20:
                    get_chbenchmark_Query16_req(req, cid);
                    break;
                case 21:
                    get_chbenchmark_Query17_req(req, cid);
                    break;
                case 22:
                    get_chbenchmark_Query18_req(req, cid);
                    break;
                case 23:
                    get_chbenchmark_Query19_req(req, cid);
                    break;
                case 24:
                    get_chbenchmark_Query20_req(req, cid);
                    break;
                case 25:
                    get_chbenchmark_Query21_req(req, cid);
                    break;
                case 26:
                    get_chbenchmark_Query22_req(req, cid);
                    break;
                default:
                    verify(0);
            }
        }
    }

}// namespace rococo
