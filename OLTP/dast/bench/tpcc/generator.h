
#pragma once

#include <deptran/config.h>
#include "__dep__.h"
#include "deptran/txn_req_factory.h"

namespace rococo {

class TpccTxnGenerator : public TxnGenerator {
 public:
  typedef struct {
    int n_w_id_;
    int n_d_id_;
    int n_c_id_;
    int n_i_id_;
    int n_r_id_;
    int const_home_w_id_;
    int delivery_d_id_;
  } tpcc_para_t;
  tpcc_para_t tpcc_para_;

 public:
  using TxnGenerator::TxnGenerator;
  TpccTxnGenerator(rococo::Config *config, parid_t partition_id);
  vector<const char*> cLastParts={
	"BAR", "OUGHT", "ABLE", "PRI", "PRES", "ESE", "ANTI", "CALLY", "ATION", "EING"};
  int randomUniformInt(int minValue, int maxValue) const{
	return (rand()%(maxValue-minValue+1))+minValue;
  }
  void randomUniformInt(int minValue, int maxValue, int& ret) const{
	ret = (rand()%(maxValue-minValue+1))+minValue;
  }
  void randomNonUniformInt(int A, int x, int y, int C, int& ret) const{
	ret = ((((randomUniformInt(0,A)|randomUniformInt(x,y))+C)%(y-x+1))+x);
  }
  void genCLast(int value, string& ret) const{
	ret = "";
	ret += cLastParts[value / 100];
	value %= 100;
	ret += cLastParts[value / 10];
	value %= 10;
	ret += cLastParts[value];
  }
  void randomCLast(string& ret) const{
	int value=0;
	randomNonUniformInt(255,0,999,173,value);
	genCLast(value,ret);
  } 
  // tpcc
  virtual void GetTxnReq(TxnRequest *req, uint32_t cid) override;
  // tpcc new_order
  void GetNewOrderTxnReq(TxnRequest *req, uint32_t cid) const;
  // tpcc payment
  void get_tpcc_payment_txn_req(TxnRequest *req, uint32_t cid) const;
  // tpcc stock_level
  void get_tpcc_stock_level_txn_req(TxnRequest *req, uint32_t cid) const;
  // tpcc delivery
  void get_tpcc_delivery_txn_req(TxnRequest *req, uint32_t cid) const;
  // tpcc order_status
  void get_tpcc_order_status_txn_req(TxnRequest *req, uint32_t cid) const;
};

} // namespace rococo

