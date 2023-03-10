#ifndef CONFIG_H_
#define CONFIG_H_

#include "__dep__.h"
#include "constants.h"
#include "sharding.h"

namespace rococo {

//#define N_PAR_PER_REGION 2

//Change to this number not tested
#define N_REP_PER_SHARD 3

class Config {
public:
    static const int BASE_CLIENT_CTRL_PORT = 5555;
    typedef enum {
        SS_DISABLED,
        SS_THREAD_SINGLE,
        SS_PROCESS_SINGLE
    } single_server_t;

    std::map<string, mdb::symbol_t> tbl_types_map_ = {
            {"sorted", mdb::TBL_SORTED},
            {"unsorted", mdb::TBL_UNSORTED},
            {"snapshot", mdb::TBL_SNAPSHOT}};

    enum ClientType { Open,
                      Closed };

public:
    int edge_client_in_region_rate_;
    bool edge_deployment_enabled_ = false;


    std::set<std::string> dc_names_;
    static Config *config_s;
    void init_hostsmap(const char *hostspath);
    std::string site2host_addr(std::string &name);
    std::string site2host_name(std::string &addr);

    bool heart_beat_;
    // configuration for trial controller.
    char *ctrl_hostname_;
    uint32_t ctrl_port_;
    uint32_t ctrl_timeout_;
    char *ctrl_key_;
    char *ctrl_init_;
    uint32_t duration_;
    vector<string> config_paths_;

    // common configuration
    ClientType client_type_ = Closed;
    int client_rate_ = -1;
    int32_t cc_mode_ = 0;
    int32_t ab_mode_ = 0;
    uint32_t proc_id_;
    int32_t benchmark_;        // workload
    uint32_t scale_factor_ = 1;// currently disabled
    std::vector<double> txn_weight_;
    map<string, double> txn_weights_;
    std::string proc_name_;
    bool batch_start_;
    bool early_return_;
    bool retry_wait_;
    string logging_path_;
    single_server_t single_server_;
    uint16_t n_concurrent_;
    int32_t max_retry_;
    string dist_ = "uniform";
    float coeffcient_ = 0;// "uniform"
    int32_t rotate_{3};
    int32_t n_parallel_dispatch_{0};
    bool forwarding_enabled_ = false;

    std::map<std::string, int> raft_port_map_;

    // TODO remove, will cause problems.
    uint32_t num_site_;
    uint32_t start_coordinator_id_;
    vector<string> site_;
    vector<uint32_t> site_threads_;
    uint32_t num_coordinator_threads_;
    uint32_t sid_;
    uint32_t cid_;

    //TPCC remote
    int tpcc_payment_crt_rate_;

    //ch benchmark remote
    int chbenchmark_payment_crt_rate_;

    enum SiteInfoType { CLIENT,
                        SERVER };
    struct SiteInfo {
        siteid_t id;       // unique site id
        uint32_t locale_id;// represents a group of servers, such as those located in same datacenter
        string name;       // site name
        string proc_name;  // proc name
        string host;
        string dcname;
        uint32_t port = 0;
        uint32_t n_thread;// should be 1 for now
        SiteInfoType type_;
        uint32_t partition_id_ = 0;

        SiteInfo() = delete;
        SiteInfo(uint32_t id) : id(id) {}
        SiteInfo(uint32_t id, std::string &site_addr) : id(id) {
            auto pos = site_addr.find(':');
            verify(pos != std::string::npos);
            name = site_addr.substr(0, pos);
            std::string port_str = site_addr.substr(pos + 1);
            port = std::stoi(port_str);
        }

        string GetBindAddress() {
            string ret("0.0.0.0:");
            ret = ret.append(std::to_string(port));
            return ret;
        }

        string GetHostAddr() {
            if (proc_name.empty())
                proc_name = name;
            if (host.empty())
                host = proc_name;
            string ret;
            ret = ret.append(host).append(":").append(std::to_string(port));
            return ret;
        }
    };


    struct ReplicaGroup {
        parid_t partition_id = 0;
        std::vector<SiteInfo *> replicas;
        ReplicaGroup(parid_t id) : partition_id(id) {}
    };

    uint32_t next_site_id_;
    vector<ReplicaGroup> replica_groups_;
    vector<SiteInfo> sites_;
    vector<SiteInfo> par_clients_;
    map<string, string> proc_host_map_;
    map<string, string> site_proc_map_;
    map<string, string> host_dc_map_;

    int n_shard_per_region_;
    int n_rep_per_shard_;

    Sharding *sharding_;

    int ov_aggregate_interval_ms_;
    int ov_gossip_interval_ms_;
    int ov_clock_drift_ms_;
    int chronos_local_sync_interval_ms_;

protected:
    Config() = default;

    Config(char *ctrl_hostname,
           uint32_t ctrl_port,
           uint32_t ctrl_timeout,
           char *ctrl_key,
           char *ctrl_init,
           uint32_t duration,
           bool heart_beat,
           single_server_t single_server,
           string logging_path);
    int GetClientPort(std::string site_name);

public:
    static int CreateConfig(int argc,
                            char **argv);
    static Config *GetConfig();
    static void DestroyConfig();

    void InitTPCCD();

    void Load();

    void LoadYML(std::string &);
    void LoadSiteYML(YAML::Node config);
    void LoadProcYML(YAML::Node config);
    void LoadHostYML(YAML::Node config);
    void LoadDatacenterYML(YAML::Node config);
    void LoadModeYML(YAML::Node config);
    void LoadBenchYML(YAML::Node config);
    void LoadShardingYML(YAML::Node config);
    void LoadClientYML(YAML::Node client);
    void LoadSchemaYML(YAML::Node config);
    void LoadSchemaTableColumnYML(Sharding::tb_info_t &tb_info,
                                  YAML::Node column);
    void LoadOVParams(YAML::Node ov);
    void LoadChronosParams(YAML::Node ov);
    void LoadEdgeConfig(YAML::Node edge);
    void LoadAuxiliaryRaft(YAML::Node raft);

    void InitMode(std::string &cc_name, string &ab_name);
    void InitBench(std::string &);

    uint32_t get_site_id();
    uint32_t get_client_id();
    uint32_t get_ctrl_port();
    uint32_t get_ctrl_timeout();
    const char *get_ctrl_hostname();
    const char *get_ctrl_key();
    const char *get_ctrl_init();

    int GetProfilePath(char *prof_file);

    // const char *get_ctrl_run();
    uint32_t get_duration();
    bool do_heart_beat();
    int32_t get_all_site_addr(std::vector<std::string> &servers);
    int32_t get_site_addr(uint32_t sid,
                          std::string &server);

    int NumSites(SiteInfoType type = SERVER);
    const SiteInfo &SiteById(uint32_t id);
    vector<SiteInfo> SitesByPartitionId(parid_t partition_id);
    vector<SiteInfo> SitesByLocaleId(uint32_t locale_id, SiteInfoType type = SERVER);
    vector<SiteInfo> SitesByProcessName(string proc_name, SiteInfoType type = SERVER);

    vector<SiteInfo> SitesInMyRegion(siteid_t site_id);


    SiteInfo *SiteByName(std::string name);
    int GetPartitionSize(parid_t par_id);
    vector<SiteInfo> GetMyServers() { return SitesByProcessName(this->proc_name_, SERVER); }
    vector<SiteInfo> GetMyClients() { return SitesByProcessName(this->proc_name_, CLIENT); }
    int NumClients() {
        return par_clients_.size();
    }

    vector<parid_t> GetAllPartitionIds() {
        vector<parid_t> ret;
        for (int i = 0; i < replica_groups_.size(); i++) {
            ret.push_back(i);
        }
        return ret;
    }

    int32_t get_threads(uint32_t &threads);
    int32_t get_mode();
    uint32_t get_num_threads();
    uint32_t get_start_coordinator_id();
    int32_t get_benchmark();
    uint32_t GetNumPartition();
    uint32_t get_scale_factor();
    int32_t get_max_retry();
    single_server_t get_single_server();
    uint32_t get_concurrent_txn();
    bool get_batch_start();
    bool do_early_return();
    bool do_logging();
    bool IsReplicated();

    const char *log_path();

    bool retry_wait();

    std::vector<double> &get_txn_weight();

    ~Config();

    map<string, double> &get_txn_weights();

    void BuildSiteProcMap(YAML::Node node);
};


bool partitions_in_same_region(parid_t a, parid_t b);

bool sites_in_same_region(siteid_t a, siteid_t b);

bool sites_for_same_par(siteid_t a, siteid_t b);

}// namespace rococo

#endif// ifndef CONFIG_H_
