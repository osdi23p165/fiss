#ifndef PARTIAL_HTAP_DELTAMANAGER_H
#define PARTIAL_HTAP_DELTAMANAGER_H
#include "utilities/_dep_.h"
#include "pbbslib/get_time.h"

class DeltaManager
{
public:
    static vector<double> tsfTime;
    static vector<double> opTime;
    static vector<double> totTime;
    static timer ts;
    static double latency;
    bool ingestBatch(int shard_id, string col_id, vector<state_transfer::Update> &batch);
    bool ingestup(int shard_id, string col_id, state_transfer::Update update, timer t);
};
#endif // PARTIAL_HTAP_DELTAMANAGER_H
