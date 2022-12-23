#include "piece.h"
#include <limits>

namespace rococo {

using rococo::RccDTxn;

char CHBENCHMARK_TB_WAREHOUSE[] =    "warehouse";
char CHBENCHMARK_TB_DISTRICT[] =     "district";
char CHBENCHMARK_TB_CUSTOMER[] =     "customer";
char CHBENCHMARK_TB_HISTORY[] =      "history";
char CHBENCHMARK_TB_ORDER[] =        "order";
char CHBENCHMARK_TB_NEW_ORDER[] =    "new_order";
char CHBENCHMARK_TB_ITEM[] =         "item";
char CHBENCHMARK_TB_STOCK[] =        "stock";
char CHBENCHMARK_TB_ORDER_LINE[] =   "order_line";
char CHBENCHMARK_TB_ORDER_C_ID_SECONDARY[] = "order_secondary";
char CHBENCHMARK_TB_REGION[] = "region";
char CHBENCHMARK_TB_NATION[] = "nation";
char CHBENCHMARK_TB_SUPPLIER[] = "supplier";

void ChbenchmarkPiece::reg_all() {
    RegNewOrder();
    RegPayment();
    RegOrderStatus();
    RegDelivery();
    RegStockLevel();
    RegQuery1();
    RegQuery2();
    RegQuery3();
    RegQuery4();
    RegQuery5();
    RegQuery6();
    RegQuery7();
    RegQuery8();
    RegQuery9();
    RegQuery10();
    RegQuery11();
    RegQuery12();
    RegQuery13();
    RegQuery14();
    RegQuery15();
    RegQuery16();
    RegQuery17();
    RegQuery18();
    RegQuery19();
    RegQuery20();
    RegQuery21();
    RegQuery22();
}

std::set<std::pair<innid_t, innid_t>> ChbenchmarkPiece::conflicts_ {};



ChbenchmarkPiece::ChbenchmarkPiece() {
  Log_info("%s called", __FUNCTION__ );
   for(int i = 0; i < 1000; i++){
     conflicts_.insert(std::make_pair(18000+i,1000));
     conflicts_.insert(std::make_pair(18000+i,303));
     conflicts_.insert(std::make_pair(18000+i,402));
     conflicts_.insert(std::make_pair(18000+i,501));
     for(int j = 0; j < 1000; j++){
       conflicts_.insert(std::make_pair(18000+i,1000+j));
       conflicts_.insert(std::make_pair(52000+i,17000+j));
     }
     conflicts_.insert(std::make_pair(303,18000+i));
     conflicts_.insert(std::make_pair(402,18000+i));
     conflicts_.insert(std::make_pair(501,18000+i));
   }
   conflicts_.insert(std::make_pair(1004,1000));
   conflicts_.insert(std::make_pair(1004,1004));
   conflicts_.insert(std::make_pair(1004,400));
   conflicts_.insert(std::make_pair(204,1002));
   conflicts_.insert(std::make_pair(204,203));
   conflicts_.insert(std::make_pair(204,204));
   conflicts_.insert(std::make_pair(204,301));
   conflicts_.insert(std::make_pair(204,302));
   conflicts_.insert(std::make_pair(204,403));
   conflicts_.insert(std::make_pair(205,205));
   conflicts_.insert(std::make_pair(301,204));
   conflicts_.insert(std::make_pair(301,403));
   conflicts_.insert(std::make_pair(302,1003));
   conflicts_.insert(std::make_pair(302,401));
   conflicts_.insert(std::make_pair(303,402));
   conflicts_.insert(std::make_pair(401,1003));
   conflicts_.insert(std::make_pair(401,302));
   conflicts_.insert(std::make_pair(401,401));
   conflicts_.insert(std::make_pair(402,303));
   conflicts_.insert(std::make_pair(402,402));
   conflicts_.insert(std::make_pair(402,501));
   conflicts_.insert(std::make_pair(403,1002));
   conflicts_.insert(std::make_pair(403,203));
   conflicts_.insert(std::make_pair(403,204));
   conflicts_.insert(std::make_pair(403,301));
   conflicts_.insert(std::make_pair(403,302));
   conflicts_.insert(std::make_pair(403,403));
   conflicts_.insert(std::make_pair(501,402));


}

ChbenchmarkPiece::~ChbenchmarkPiece() {}

}
