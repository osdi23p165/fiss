#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include "utilities/ThreadPool.h"
#include "utilities/Graph.h"
#include "utilities/Constant.h"
#include "Config.h"
#include "pbbslib/get_time.h"
#include "PAM/tpch/test.h"
#include "DeltaManager.h"

enum Status
{
    Pending = 1,
    Waiting = 2,
    QueryProcessing = 3,
    TransactionProcessing = 4
};
typedef struct
{
    int status;
    int up_cnt;
    ulong estimated_time;
} odVertex;

class ThreadPool;
class APServer final : public state_transfer::OLAPServer::Service
{
public:
    string server_address_ = "0.0.0.0:51209";
    //static std::map<int, std::map<string, enum Status>> orderStatus;
    //<shard_ID<col_ID,queue<Order>>>
    static std::map<int, std::map<string, Graph<ulong>>> orderMap;
    static std::map<int, std::map<string, std::map<ulong, odVertex>>> orderVec;
    //<shard_ID<col_ID,<Txn_ID,vector<Update>>>>
    static std::map<int, std::map<string, std::map<ulong, vector<state_transfer::Update>>>> updateMap;
    ThreadPool *threadPool;
    static DeltaManager *deltaManager;
    //std::mutex is neither copyable nor movable
    static std::mutex mtxUpMap[30][40]; //hardcode
    static std::mutex mtxOdMap[30][40]; //hardcode
    static std::mutex mu_waitlist_;
    static std::map<ulong, std::map<int, std::map<string, int>>> waitlist_;
    APServer(DeltaManager *deltaManager, ThreadPool *thread_pool)
    {
        this->deltaManager = deltaManager;
        this->threadPool = thread_pool;
        //initialize orderVec
        for (int i = 0; i < tp_info_.ShardList.size(); i++)
        {
            std::map<string, std::map<ulong, odVertex>> perShardMap;
            for (int j = 0; j < tp_info_.ColIDList.size(); j++)
            {
                std::map<ulong, odVertex> perColMap;
                perShardMap.insert(std::make_pair(tp_info_.ColIDList[j], perColMap));
            }
            orderVec.insert(std::make_pair(tp_info_.ShardList[i], perShardMap));
        }
        //initialize orderMap
        for (int i = 0; i < tp_info_.ShardList.size(); i++)
        {
            std::map<string, Graph<ulong>> perShardMap;
            for (int j = 0; j < tp_info_.ColIDList.size(); j++)
            {
                Graph<ulong> perColMap;
                perShardMap.insert(std::make_pair(tp_info_.ColIDList[j], perColMap));
            }
            orderMap.insert(std::make_pair(tp_info_.ShardList[i], perShardMap));
        }
        //initialize updateMap
        for (int i = 0; i < tp_info_.ShardList.size(); i++)
        {
            std::map<string, std::map<ulong, vector<state_transfer::Update>>> perShardMap;
            for (int j = 0; j < tp_info_.ColIDList.size(); j++)
            {
                std::map<ulong, vector<state_transfer::Update>> perColMap;
                perShardMap.insert(std::make_pair(tp_info_.ColIDList[j], perColMap));
            }
            updateMap.insert(std::make_pair(tp_info_.ShardList[i], perShardMap));
        }
        //std::thread(&APServer::printOdMap, this).detach();
        //std::thread(&APServer::printUpMap, this).detach();
    }
    string get_server_address_()
    {
        return server_address_;
    };
    static bool checkAndShipToReadyQueue(int shard_id, string col_id);
    grpc::Status TransferUpdate(grpc::ServerContext *context,
                                grpc::ServerReaderWriter<state_transfer::UpdateResp,
                                                         state_transfer::UpdateReq> *stream) override;

    grpc::Status TransferOrder(grpc::ServerContext *context,
                               grpc::ServerReaderWriter<state_transfer::OrderResp,
                                                        state_transfer::OrderReq> *stream) override;

    static void backUpdate(int shard_id, string col_id, state_transfer::Update up);
    static bool ApplyUpdateByOrder(int shard_id, string col_id, ulong tid);
    static bool TriggerCheckParent(int shard_id, string col_id, ulong tid);
    static void backOrder(int shard_id, string col_id, state_transfer::Order od);
    void printUpMap();
    void printOdMap();
    void query();
    void TriggerCleanWaitlist();
};
