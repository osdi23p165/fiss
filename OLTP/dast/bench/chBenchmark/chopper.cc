#include "deptran/__dep__.h"
#include "deptran/txn_chopper.h"
#include "chopper.h"
#include "piece.h"

namespace rococo {


#define CHBENCHMARK_DELIVERY_Ith_INDEX_NEW_ORDER(i)    (4 * i)
#define CHBENCHMARK_DELIVERY_Ith_INDEX_ORDER(i)        (1 + 4 * i)
#define CHBENCHMARK_DELIVERY_Ith_INDEX_ORDER_LINE(i)   (2 + 4 * i)
#define CHBENCHMARK_DELIVERY_Ith_INDEX_CUSTOMER(i)     (3 + 4 * i)
#define CHBENCHMARK_DELIVERY_IS_NEW_ORDER_INDEX(i)     ((i % 4) == 0)
#define CHBENCHMARK_DELIVERY_IS_ORDER_INDEX(i)         ((i % 4) == 1)
#define CHBENCHMARK_DELIVERY_IS_ORDER_LINE_INDEX(i)    ((i % 4) == 2)
#define CHBENCHMARK_DELIVERY_IS_CUSTOMER(i)            ((i % 4) == 3)
#define CHBENCHMARK_DELIVERY_INDEX_NEW_ORDER_TO_CNT(i)         (i / 4)
#define CHBENCHMARK_DELIVERY_INDEX_NEW_ORDER_TO_ORDER(i)       (i + 1)
#define CHBENCHMARK_DELIVERY_INDEX_NEW_ORDER_TO_ORDER_LINE(i)  (i + 2)
#define CHBENCHMARK_DELIVERY_INDEX_NEW_ORDER_TO_CUSTOMER(i)    (i + 3)
#define CHBENCHMARK_DELIVERY_INDEX_ORDER_TO_CNT(i)             ((i - 1) / 4)
#define CHBENCHMARK_DELIVERY_INDEX_ORDER_TO_CUSTOMER(i)        (i + 2)
#define CHBENCHMARK_DELIVERY_INDEX_ORDER_LINE_TO_CNT(i)        ((i - 2) / 4)
#define CHBENCHMARK_DELIVERY_INDEX_ORDER_LINE_TO_CUSTOMER(i)   (i + 1)

    ChbenchmarkTxn::ChbenchmarkTxn() {
    }

    void ChbenchmarkTxn::Init(TxnRequest &req) {
        ws_init_ = req.input_;
        ws_ = ws_init_;
        type_ = req.txn_type_;
        callback_ = req.callback_;
        max_try_ = req.n_try_;
        n_try_ = 1;
        isQuery = req.isQuery;
        touchColsCnt = req.touchColsCnt;
        txn_type = req.txn_type_;
        commit_.store(true);
        input_vars_ = txn_reg_->input_vars_[type_];
        switch (type_) {
            case CHBENCHMARK_NEW_ORDER:
                NewOrderInit(req);
                break;
            case CHBENCHMARK_PAYMENT:
                PaymentInit(req);
                break;
            case CHBENCHMARK_ORDER_STATUS:
                OrderStatusInit(req);
                break;
            case CHBENCHMARK_DELIVERY:
                DeliveryInit(req);
                break;
            case CHBENCHMARK_STOCK_LEVEL:
                StockLevelInit(req);
                break;
            case CHBENCHMARK_QUERY1:
                Query1Init(req);
                break;
            case CHBENCHMARK_QUERY2:
                Query2Init(req);
                break;
            case CHBENCHMARK_QUERY3:
                Query3Init(req);
                break;
            case CHBENCHMARK_QUERY4:
                Query4Init(req);
                break;
            case CHBENCHMARK_QUERY5:
                Query5Init(req);
                break;
            case CHBENCHMARK_QUERY6:
                Query6Init(req);
                break;
            case CHBENCHMARK_QUERY7:
                Query7Init(req);
                break;
            case CHBENCHMARK_QUERY8:
                Query8Init(req);
                break;
            case CHBENCHMARK_QUERY9:
                Query9Init(req);
                break;
            case CHBENCHMARK_QUERY10:
                Query10Init(req);
                break;
            case CHBENCHMARK_QUERY11:
                Query11Init(req);
                break;
            case CHBENCHMARK_QUERY12:
                Query12Init(req);
                break;
            case CHBENCHMARK_QUERY13:
                Query13Init(req);
                break;
            case CHBENCHMARK_QUERY14:
                Query14Init(req);
                break;
            case CHBENCHMARK_QUERY15:
                Query15Init(req);
                break;
            case CHBENCHMARK_QUERY16:
                Query16Init(req);
                break;
            case CHBENCHMARK_QUERY17:
                Query17Init(req);
                break;
            case CHBENCHMARK_QUERY18:
                Query18Init(req);
                break;
            case CHBENCHMARK_QUERY19:
                Query19Init(req);
                break;
            case CHBENCHMARK_QUERY20:
                Query20Init(req);
                break;
            case CHBENCHMARK_QUERY21:
                Query21Init(req);
                break;
            case CHBENCHMARK_QUERY22:
                Query22Init(req);
                break;
            default:
                verify(0);
        }
        verify(n_pieces_dispatchable_ > 0);
    }

// This is sort of silly. We should have a better way.
    bool ChbenchmarkTxn::CheckReady() {
        Log_info("%s called", __PRETTY_FUNCTION__);
        bool ret = false;
        map <innid_t, set<int32_t>> &map = input_vars_;
        for (auto &pair : status_) {
            const innid_t &pi = pair.first;
            int32_t &status = pair.second;
            if (status != WAITING) {
                continue;
            }
            set <int32_t> &var_set = map[pi];
            bool all_found = true;
            for (auto &var : var_set) {
                if (ws_.count(var) == 0) {
                    // not found. input not all ready.
                    all_found = false;
                    break;
                } else {
#ifdef DEBUG_CODE
                    TxnWorkspace& ws = GetWorkspace(pi);
                    if (ws.keys_.size() == 0)
                      ws.keys_ = var_set;
                    verify(ws_[var].get_kind() != 0);
#endif
                }
            }
            // all found.
            if (all_found && status == WAITING) {
                Log_debug("Changing piece %d to dispatchable", pi);
                status = DISPATCHABLE;
                TxnWorkspace &ws = GetWorkspace(pi);
                ws.keys_ = var_set;
                n_pieces_dispatchable_++;
                ret = true;
            }
        }
        return ret;
    }

    void ChbenchmarkTxn::PrepareAllCmds() {
        for (auto &pair: status_) {
            const innid_t &pi = pair.first;
            int32_t &status = pair.second;

            if (status == WAITING) {
//       status = DISPATCHABLE;
                //The original GetWorkSpace ensures that a txn's ws contains all required input.
                //Now, this cannot esnure.
                TxnWorkspace &ws = GetWorkspace(pi);
                ws.keys_ = input_vars_[pi];
            }
        }
    }


    bool ChbenchmarkTxn::start_callback(int pi,
                                        int res,
                                        map <int32_t, Value> &output_map) {
        bool ret;

        ws_.insert(output_map);
        if (type_ == CHBENCHMARK_PAYMENT ||
            type_ == CHBENCHMARK_ORDER_STATUS ||
            type_ == CHBENCHMARK_DELIVERY ||
            type_ == CHBENCHMARK_NEW_ORDER ||
            0) {
            // for debug

            auto ret = CheckReady();
            if (type_ == CHBENCHMARK_DELIVERY) {
                if (pi == CHBENCHMARK_DELIVERY_2) {
                    verify(output_map.count(CHBENCHMARK_VAR_OL_AMOUNT) > 0);
                    verify(ws_.count(CHBENCHMARK_VAR_OL_AMOUNT) > 0);
                }
                if (pi == CHBENCHMARK_DELIVERY_1) {
                    verify(output_map.count(CHBENCHMARK_VAR_C_ID) > 0);
                    verify(ws_.count(CHBENCHMARK_VAR_C_ID) > 0);
                }
            }
            Log_info("type_ %d, here1", type_);
            return ret;
        }
        PieceCallbackHandler handler;
        auto it = txn_reg_->callbacks_.find(std::make_pair(type_, pi));
        if (it != txn_reg_->callbacks_.end()) {
            handler = it->second;
            ret = handler(this, output_map);
            Log_info("type_ %d, here1", type_);
        } else {
            // ws_.insert(output_map.begin(), output_map.end());
            bool ret = CheckReady();
            Log_info("type_ %d, here1", type_);
        }

        // below is for debug
        if (type_ == CHBENCHMARK_STOCK_LEVEL && pi == CHBENCHMARK_STOCK_LEVEL_0) {
            verify(ws_.count(CHBENCHMARK_VAR_D_NEXT_O_ID) > 0);
            TxnWorkspace &ws = GetWorkspace(CHBENCHMARK_STOCK_LEVEL_1);
            verify(ws.count(CHBENCHMARK_VAR_D_NEXT_O_ID) > 0);
            verify(status_[CHBENCHMARK_STOCK_LEVEL_1] == DISPATCHABLE);
        }
        return ret;
    }


    void ChbenchmarkTxn::Reset() {
        TxnCommand::Reset();
        ws_ = ws_init_;
        partition_ids_.clear();
        n_try_++;
        commit_.store(true);
        n_pieces_dispatchable_ = 0;
        n_pieces_dispatch_acked_ = 0;
        n_pieces_dispatched_ = 0;
        switch (type_) {
            case CHBENCHMARK_NEW_ORDER:
                NewOrderRetry();
                break;
            case CHBENCHMARK_PAYMENT:
                PaymentRetry();
                break;
            case CHBENCHMARK_ORDER_STATUS:
                OrderStatusRetry();
                break;
            case CHBENCHMARK_DELIVERY:
                DeliveryRetry();
                break;
            case CHBENCHMARK_STOCK_LEVEL:
                StockLevelRetry();
                break;
            case CHBENCHMARK_QUERY1:
                Query1Retry();
                break;
            case CHBENCHMARK_QUERY2:
                Query2Retry();
                break;
            case CHBENCHMARK_QUERY3:
                Query3Retry();
                break;
            case CHBENCHMARK_QUERY4:
                Query4Retry();
                break;
            case CHBENCHMARK_QUERY5:
                Query5Retry();
                break;
            case CHBENCHMARK_QUERY6:
                Query6Retry();
                break;
            case CHBENCHMARK_QUERY7:
                Query7Retry();
                break;
            case CHBENCHMARK_QUERY8:
                Query8Retry();
                break;
            case CHBENCHMARK_QUERY9:
                Query9Retry();
                break;
            case CHBENCHMARK_QUERY10:
                Query10Retry();
                break;
            case CHBENCHMARK_QUERY11:
                Query11Retry();
                break;
            case CHBENCHMARK_QUERY12:
                Query12Retry();
                break;
            case CHBENCHMARK_QUERY13:
                Query13Retry();
                break;
            case CHBENCHMARK_QUERY14:
                Query14Retry();
                break;
            case CHBENCHMARK_QUERY15:
                Query15Retry();
                break;
            case CHBENCHMARK_QUERY16:
                Query16Retry();
                break;
            case CHBENCHMARK_QUERY17:
                Query17Retry();
                break;
            case CHBENCHMARK_QUERY18:
                Query18Retry();
                break;
            case CHBENCHMARK_QUERY19:
                Query19Retry();
                break;
            case CHBENCHMARK_QUERY20:
                Query20Retry();
                break;
            case CHBENCHMARK_QUERY21:
                Query21Retry();
                break;
            case CHBENCHMARK_QUERY22:
                Query22Retry();
                break;
            default:
                verify(0);
        }
        verify(n_pieces_dispatchable_ > 0);
    }

    bool ChbenchmarkTxn::IsReadOnly() {
        switch (type_) {
            case CHBENCHMARK_NEW_ORDER:
                return false;
            case CHBENCHMARK_PAYMENT:
                return false;
            case CHBENCHMARK_ORDER_STATUS:
                return true;
            case CHBENCHMARK_DELIVERY:
                return false;
            case CHBENCHMARK_STOCK_LEVEL:
                return true;
            case CHBENCHMARK_QUERY1:
                return true;
            case CHBENCHMARK_QUERY2:
                return true;
            case CHBENCHMARK_QUERY3:
                return true;
            case CHBENCHMARK_QUERY4:
                return true;
            case CHBENCHMARK_QUERY5:
                return true;
            case CHBENCHMARK_QUERY6:
                return true;
            case CHBENCHMARK_QUERY7:
                return true;
            case CHBENCHMARK_QUERY8:
                return true;
            case CHBENCHMARK_QUERY9:
                return true;
            case CHBENCHMARK_QUERY10:
                return true;
            case CHBENCHMARK_QUERY11:
                return true;
            case CHBENCHMARK_QUERY12:
                return true;
            case CHBENCHMARK_QUERY13:
                return true;
            case CHBENCHMARK_QUERY14:
                return true;
            case CHBENCHMARK_QUERY15:
                return true;
            case CHBENCHMARK_QUERY16:
                return true;
            case CHBENCHMARK_QUERY17:
                return true;
            case CHBENCHMARK_QUERY18:
                return true;
            case CHBENCHMARK_QUERY19:
                return true;
            case CHBENCHMARK_QUERY20:
                return true;
            case CHBENCHMARK_QUERY21:
                return true;
            case CHBENCHMARK_QUERY22:
                return true;
            default:
                verify(0);
        }
    }

    parid_t ChbenchmarkTxn::GetPiecePartitionId(innid_t inn_id) {
        parid_t partition_id = 0;
        auto it = txn_reg_->sharding_input_.find(std::make_pair(type_, inn_id));
        if (it != txn_reg_->sharding_input_.end()) {
            auto &pair = it->second;
            auto tb = pair.first;
            auto &var_ids = pair.second;
            vector <Value> vars;
            for (auto var_id : var_ids) {
                verify(ws_.count(var_id) != 0);
                vars.push_back(ws_.at(var_id));
            }
            MultiValue mv = MultiValue(vars);
            sss_->GetPartition(tb, mv, partition_id);
        } else {
            verify(0);
            partition_id = sharding_[inn_id];
        }
        return partition_id;
    }

    int ChbenchmarkTxn::GetNPieceAll() {
        if (type_ == CHBENCHMARK_STOCK_LEVEL) {
            verify(ws_.count(CHBENCHMARK_VAR_OL_AMOUNT) > 0 == ws_.count(CHBENCHMARK_VAR_N_PIECE_ALL) > 0);
            if (ws_.count(CHBENCHMARK_VAR_OL_AMOUNT) > 0) {
                verify(ws_[CHBENCHMARK_VAR_N_PIECE_ALL].get_i32() == n_pieces_all_);
                return n_pieces_all_;
                return n_pieces_all_ + ws_[CHBENCHMARK_VAR_OL_AMOUNT].get_i32();
            } else {
                return n_pieces_all_;
            }
        }
        return n_pieces_all_;
    }

    ChbenchmarkTxn::~ChbenchmarkTxn() {
    }

} // namespace rococo
