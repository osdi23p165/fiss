#pragma once

#include "deptran/__dep__.h"
#include "bench/chBenchmark/piece.h"

namespace rococo {

    class TxnCommand;

    class TxnRequest;

    class BatchStartArgsHelper;

    class ChbenchmarkTxn : public TxnCommand {
    public:
        map <innid_t, set<int32_t>> input_vars_ = {};
        typedef struct {
            size_t ol_cnt;
            bool piece_0_dist;
            bool *piece_items;
            bool *piece_stocks;
        } new_order_dep_t;

        typedef struct {
            bool piece_warehouse;
            bool piece_district;
            bool piece_last2id;
            bool piece_ori_last2id;
        } payment_dep_t;

        typedef struct {
            bool piece_last2id;
            bool piece_ori_last2id;
            bool piece_order;
        } order_status_dep_t;

        //typedef struct {
        //    size_t d_cnt;
        //    bool *piece_new_orders;
        //    bool *piece_orders;
        //    bool *piece_order_lines;
        //} delivery_dep_t;
        typedef struct {
            bool piece_new_orders;
            bool piece_orders;
            bool piece_order_lines;
        } delivery_dep_t;

        typedef struct {
            int threshold;
            int w_id;
        } stock_level_dep_t;

        union {
            new_order_dep_t new_order_dep_;
            payment_dep_t payment_dep_;
            order_status_dep_t order_status_dep_;
            delivery_dep_t delivery_dep_;
            stock_level_dep_t stock_level_dep_;
        };

        virtual void NewOrderInit(TxnRequest &req);

        virtual void NewOrderRetry();

//  // payment
//  virtual void payment_shard(const char *tb,
//                             map<int32_t, Value> &input,
//                             uint32_t &site);

        virtual void PaymentInit(TxnRequest &req);

        virtual void PaymentRetry();

        virtual void StockLevelInit(TxnRequest &req);

        virtual void StockLevelRetry();

        // delivery
        virtual void DeliveryInit(TxnRequest &req);

        virtual void DeliveryRetry();

        // order status
//  virtual void order_status_shard(const char *tb,
//                                  map<int32_t, Value> &input,
//                                  uint32_t &site);

        virtual void OrderStatusInit(TxnRequest &req);

        virtual void OrderStatusRetry();

        //Query1
        virtual void Query1Init(TxnRequest &req);

        virtual void Query1Retry();

        //Query2
        virtual void Query2Init(TxnRequest &req);

        virtual void Query2Retry();

        //Query3
        virtual void Query3Init(TxnRequest &req);

        virtual void Query3Retry();

        //Query4
        virtual void Query4Init(TxnRequest &req);

        virtual void Query4Retry();

        //Query5
        virtual void Query5Init(TxnRequest &req);

        virtual void Query5Retry();

        //Query6
        virtual void Query6Init(TxnRequest &req);

        virtual void Query6Retry();

        //Query7
        virtual void Query7Init(TxnRequest &req);

        virtual void Query7Retry();

        //Query8
        virtual void Query8Init(TxnRequest &req);

        virtual void Query8Retry();

        //Query9
        virtual void Query9Init(TxnRequest &req);

        virtual void Query9Retry();

        //Query10
        virtual void Query10Init(TxnRequest &req);

        virtual void Query10Retry();

        //Query11
        virtual void Query11Init(TxnRequest &req);

        virtual void Query11Retry();

        //Query12
        virtual void Query12Init(TxnRequest &req);

        virtual void Query12Retry();

        //Query13
        virtual void Query13Init(TxnRequest &req);

        virtual void Query13Retry();

        //Query14
        virtual void Query14Init(TxnRequest &req);

        virtual void Query14Retry();

        //Query15
        virtual void Query15Init(TxnRequest &req);

        virtual void Query15Retry();

        //Query16
        virtual void Query16Init(TxnRequest &req);

        virtual void Query16Retry();

        //Query17
        virtual void Query17Init(TxnRequest &req);

        virtual void Query17Retry();

        //Query18
        virtual void Query18Init(TxnRequest &req);

        virtual void Query18Retry();

        //Query19
        virtual void Query19Init(TxnRequest &req);

        virtual void Query19Retry();

        //Query20
        virtual void Query20Init(TxnRequest &req);

        virtual void Query20Retry();

        //Query21
        virtual void Query21Init(TxnRequest &req);

        virtual void Query21Retry();

        //Query22
        virtual void Query22Init(TxnRequest &req);

        virtual void Query22Retry();

    public:
        ChbenchmarkTxn();

        virtual parid_t GetPiecePartitionId(innid_t inn_id);

        virtual void Init(TxnRequest &req);


        virtual void PrepareAllCmds() override;

        virtual bool start_callback(int pi,
                                    int res,
                                    map <int32_t, Value> &output);

        virtual bool IsReadOnly();

        virtual void Reset();

        virtual bool CheckReady();

        virtual int GetNPieceAll();

        virtual ~ChbenchmarkTxn();
    };

}
