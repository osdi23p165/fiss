#include "DeltaManager.h"
#include "PAM/tpch/test.h"
#include <mutex>

vector<double> DeltaManager::tsfTime;
vector<double> DeltaManager::opTime;
vector<double> DeltaManager::totTime;
timer DeltaManager::ts;
double DeltaManager::latency;
std::mutex timeMtx;

bool DeltaManager::ingestBatch(int shard_id, string col_id, vector<state_transfer::Update> &batch)
{   //batch commit
    for (state_transfer::Update &update : batch)
    {
        string table_name = col_id.substr(0, col_id.find("."));
        string col_name = col_id.substr(col_id.find(".") + 1, col_id.length());
        op(update.op_type(), update.txnid(), table_name, update.pkey_col_name(), update.pkey_val(), col_name, update.value());
    }
    return true;
}

bool DeltaManager::ingestup(int shard_id, string col_id, state_transfer::Update update, timer t)
{
    string table_name = col_id.substr(0, col_id.find("."));
    string col_name = col_id.substr(col_id.find(".") + 1, col_id.length());

    double tsf_time = t.stop();
    double op_time = op(update.op_type(), update.txnid(), table_name, update.pkey_col_name(), update.pkey_val(), col_name, update.value());
    double tot_time = t.stop();
    double lat = ts.stop();

    timeMtx.lock();
    tsfTime.push_back(tsf_time);
    opTime.push_back(op_time);
    totTime.push_back(tot_time);
    latency = lat;
    timeMtx.unlock();

    return true;
}