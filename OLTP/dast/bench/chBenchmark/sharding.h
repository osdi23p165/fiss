

#pragma once

#include "deptran/__dep__.h"
#include "deptran/sharding.h"
#include "bench/chBenchmark/piece.h"

namespace rococo {

    class ChBenchmarkSharding : public Sharding {
    public:
        map <string, string> tbl_name_path_map = {{CHBENCHMARK_TB_WAREHOUSE,  "/htap/csv/WAREHOUSE.tbl"},
                                                  {CHBENCHMARK_TB_DISTRICT,   "/htap/csv/DISTRICT.tbl"},
                                                  {CHBENCHMARK_TB_CUSTOMER,   "/htap/csv/CUSTOMER.tbl"},
                                                  {CHBENCHMARK_TB_HISTORY,    "/htap/csv/HISTORY.tbl"},
                                                  {CHBENCHMARK_TB_ORDER,      "/htap/csv/ORDER.tbl"},
                                                  {CHBENCHMARK_TB_NEW_ORDER,  "/htap/csv/NEWORDER.tbl"},
                                                  {CHBENCHMARK_TB_ITEM,       "/htap/csv/ITEM.tbl"},
                                                  {CHBENCHMARK_TB_STOCK,      "/htap/csv/STOCK.tbl"},
                                                  {CHBENCHMARK_TB_ORDER_LINE, "/htap/csv/ORDERLINE.tbl"},
                                                  {CHBENCHMARK_TB_REGION,     "/htap/csv/REGION.tbl"},
                                                  {CHBENCHMARK_TB_NATION,     "/htap/csv/NATION.tbl"},
                                                  {CHBENCHMARK_TB_SUPPLIER,   "/htap/csv/SUPPLIER.tbl"}};

        std::multimap<c_last_id_t, rrr::i32> g_c_last2id; // XXX hardcode
        mdb::Schema g_c_last_schema;                      // XXX

        void PreparePrimaryColumn(tb_info_t *tb_info,
                                  uint32_t col_index,
                                  mdb::Schema::iterator &col_it) override {
            verify(0);
        };
        bool GenerateRowData(tb_info_t *tb_info,
                             uint32_t &sid,
                             Value &key_value,
                             vector<Value> &row_data) override {verify(0);};

        void InsertRowData(tb_info_t *tb_info,
                           uint32_t &partition_id,
                           Value &key_value,
                           const mdb::Schema *schema,
                           mdb::Table *const table_ptr,
                           mdb::SortedTable *tbl_sec_ptr,
                           vector<Value> &row_data) override {verify(0);};

//  bool Ready2Populate(tb_info_t *tb_info) override {verify(0);};


        int PopulateTable(tb_info_t *tb_info, parid_t par_id) override;
        int PopulateTables(parid_t par_id) override;

    };

} // namespace rococo