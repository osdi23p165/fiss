
#pragma once

#include <deptran/config.h>
#include "__dep__.h"
#include "deptran/txn_req_factory.h"

namespace rococo {

    class ChbenchmarkTxnGenerator : public TxnGenerator {
    public:
        typedef struct {
            int n_w_id_;
            int n_d_id_;
            int n_c_id_;
            int n_i_id_;
            int n_r_id_;
            int const_home_w_id_;
            int delivery_d_id_;
        } chbenchmark_para_t;
        chbenchmark_para_t chbenchmark_para_;

    public:
        using TxnGenerator::TxnGenerator;

        ChbenchmarkTxnGenerator(rococo::Config *config, parid_t partition_id);

        vector<const char *> cLastParts = {
                "BAR", "OUGHT", "ABLE", "PRI", "PRES", "ESE", "ANTI", "CALLY", "ATION", "EING"};

        int randomUniformInt(int minValue, int maxValue) const {
            return (rand() % (maxValue - minValue + 1)) + minValue;
        }

        void randomUniformInt(int minValue, int maxValue, int &ret) const {
            ret = (rand() % (maxValue - minValue + 1)) + minValue;
        }

        void randomNonUniformInt(int A, int x, int y, int C, int &ret) const {
            ret = ((((randomUniformInt(0, A) | randomUniformInt(x, y)) + C) % (y - x + 1)) + x);
        }

        void genCLast(int value, string &ret) const {
            ret = "";
            ret += cLastParts[value / 100];
            value %= 100;
            ret += cLastParts[value / 10];
            value %= 10;
            ret += cLastParts[value];
        }

        void randomCLast(string &ret) const {
            int value = 0;
            randomNonUniformInt(255, 0, 999, 173, value);
            genCLast(value, ret);
        }

        // chbenchmark
        virtual void GetTxnReq(TxnRequest *req, uint32_t cid) override;

        // chbenchmark new_order
        void GetNewOrderTxnReq(TxnRequest *req, uint32_t cid) const;

        // chbenchmark payment
        void get_chbenchmark_payment_txn_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark stock_level
        void get_chbenchmark_stock_level_txn_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark delivery
        void get_chbenchmark_delivery_txn_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark order_status
        void get_chbenchmark_order_status_txn_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query1
        void get_chbenchmark_Query1_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query2
        void get_chbenchmark_Query2_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query3
        void get_chbenchmark_Query3_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query4
        void get_chbenchmark_Query4_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query5
        void get_chbenchmark_Query5_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query6
        void get_chbenchmark_Query6_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query7
        void get_chbenchmark_Query7_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query8
        void get_chbenchmark_Query8_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query9
        void get_chbenchmark_Query9_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query10
        void get_chbenchmark_Query10_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query11
        void get_chbenchmark_Query11_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query12
        void get_chbenchmark_Query12_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query13
        void get_chbenchmark_Query13_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query14
        void get_chbenchmark_Query14_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query15
        void get_chbenchmark_Query15_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query16
        void get_chbenchmark_Query16_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query17
        void get_chbenchmark_Query17_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query18
        void get_chbenchmark_Query18_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query19
        void get_chbenchmark_Query19_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query20
        void get_chbenchmark_Query20_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query21
        void get_chbenchmark_Query21_req(TxnRequest *req, uint32_t cid) const;

        // chbenchmark Query22
        void get_chbenchmark_Query22_req(TxnRequest *req, uint32_t cid) const;
    };

} // namespace rococo

