#include "sharding.h"
#include "bench/chBenchmark/piece.h"
#include "deptran/frame.h"
#include <unistd.h>

namespace rococo {

    int ChBenchmarkSharding::PopulateTable(tb_info_t *tb_info_ptr, parid_t par_id) {

        //  Log_info("%s called", __PRETTY_FUNCTION__);
//        Log_info("%s called, Populate table name: %s, Partition id: %d", __FUNCTION__, tb_info_ptr->tb_name.c_str(),
//                 par_id);


        // Get the path of table
        auto path_iter = tbl_name_path_map.find(tb_info_ptr->tb_name);
        if (path_iter == tbl_name_path_map.end()) {
            Log_fatal("Table %s does not belong to chBenchmark schema", tb_info_ptr->tb_name.c_str());
        }
        string tbl_path = path_iter->second;

        vector <vector<string>> table;
        tbl_parser(tbl_path, table);

//        Log_info("Tabel Row Number: %d, Tabel Col Number: %d", table.size(), tb_info_ptr->columns.size());

        mdb::Table *const table_ptr = dtxn_sched_->get_table(tb_info_ptr->tb_name);
        const mdb::Schema *schema = table_ptr->schema();
        verify(schema->columns_count() == tb_info_ptr->columns.size());
//        Log_info("Table schema key_columns_id size: %d", (int) table_ptr->schema()->key_columns_id().size());
        uint32_t row_index = 0;
        uint32_t col_index = 0;
        std::vector <Value> row_data;
        bool verified_success;

        unsigned int self_primary_col = 0;
        vector<unsigned int> w_id_cols;
        bool self_primary_col_find = false;
        mdb::Schema::iterator col_it = schema->begin();
        unsigned int cnt = 0;

        // for table [region, nation], populate it directly
        if (tb_info_ptr->tb_name == CHBENCHMARK_TB_REGION || tb_info_ptr->tb_name == CHBENCHMARK_TB_NATION) {
            // Verify schema and Find self_primary_col
            for (col_index = 0; col_index < tb_info_ptr->columns.size(); ++col_index) {
                verify(col_it != schema->end());
                verify(tb_info_ptr->columns[col_index].name == col_it->name);
                verify(tb_info_ptr->columns[col_index].type == col_it->type);

                if (tb_info_ptr->columns[col_index].is_primary) {
                    verify(col_it->indexed);
                    if (tb_info_ptr->columns[col_index].foreign_tb == NULL) {
                        // only one primary key can refer to no other table.
                        verify(!self_primary_col_find);
                        self_primary_col = col_index;
                        self_primary_col_find = true;
                    }
                }
                col_it++;
            }

            // Traverse the row and col to insert data
            for (row_index = 0; row_index < table.size(); ++row_index) {
                row_data.clear();
                for (col_index = 0; col_index < tb_info_ptr->columns.size(); ++col_index) {
                    verified_success = true;
//                    Log_info("Row index: %d, Col index: %d", row_index, col_index);
                    Value v_buf = value_parser(tb_info_ptr->columns[col_index].type, table[row_index][col_index]);

                    // Record the foreign values
                    if (tb_info_ptr->columns[col_index].values != NULL) {
                        tb_info_ptr->columns[col_index].values->push_back(v_buf);
                    }

//                    Log_info("Insert row: %s", table[row_index][col_index].c_str());
                    row_data.push_back(value_parser(tb_info_ptr->columns[col_index].type, table[row_index][col_index]));
                }

                // insert data
                if (col_index == tb_info_ptr->columns.size()) {
                    cnt++;
                    mdb::Row *r = frame_->CreateRow(schema, row_data);
                    table_ptr->insert(r);
                }
            }
        }

        // for table [item, supplier, ], use pkey to do the partition
        else if (tb_info_ptr->tb_name == CHBENCHMARK_TB_ITEM || tb_info_ptr->tb_name == CHBENCHMARK_TB_SUPPLIER ) {
            // Verify schema and Find self_primary_col
            for (col_index = 0; col_index < tb_info_ptr->columns.size(); ++col_index) {
                verify(col_it != schema->end());
                verify(tb_info_ptr->columns[col_index].name == col_it->name);
                verify(tb_info_ptr->columns[col_index].type == col_it->type);

                if (tb_info_ptr->columns[col_index].is_primary) {
                    verify(col_it->indexed);
                    if (tb_info_ptr->columns[col_index].foreign_tb == NULL) {
                        // only one primary key can refer to no other table.
                        verify(!self_primary_col_find);
                        self_primary_col = col_index;
                        self_primary_col_find = true;
                    }
                }
                col_it++;
            }

            // Traverse the row and col to insert data
            for (row_index = 0; row_index < table.size(); ++row_index) {
                row_data.clear();
                for (col_index = 0; col_index < tb_info_ptr->columns.size(); ++col_index) {
                    verified_success = true;
//                    Log_info("Row index: %d, Col index: %d", row_index, col_index);
                    Value v_buf = value_parser(tb_info_ptr->columns[col_index].type, table[row_index][col_index]);
                    // self primary col, to check the partition
                    if (col_index == self_primary_col) {
                        if (par_id != PartitionFromKey(v_buf, tb_info_ptr)) {
//                            Log_info("Not this partition, row_id: %s, should in %d", table[row_index][col_index].c_str(), PartitionFromKey(v_buf, tb_info_ptr));
                            verified_success = false;
                        }
                    }
                    if (!verified_success) {
                        break;
                    }

                    // Record the foreign values
                    if (tb_info_ptr->columns[col_index].values != NULL) {
                        tb_info_ptr->columns[col_index].values->push_back(v_buf);
                    }

//                    Log_info("Insert row: %s", table[row_index][col_index].c_str());
                    row_data.push_back(value_parser(tb_info_ptr->columns[col_index].type, table[row_index][col_index]));
                }

                // insert data
                if (col_index == tb_info_ptr->columns.size()) {
                    cnt++;
                    mdb::Row *r = frame_->CreateRow(schema, row_data);
                    table_ptr->insert(r);
                }
            }
        }

        else {
            verify(w_id_cols.size() == 0);

            mdb::SortedTable *tbl_sec_ptr = NULL;
            if (tb_info_ptr->tb_name == CHBENCHMARK_TB_ORDER) {
                tbl_sec_ptr = (mdb::SortedTable *) dtxn_sched_->get_table(CHBENCHMARK_TB_ORDER_C_ID_SECONDARY);
            }

            // Verify schema and Find w_id_col
            for (col_index = 0; col_index < tb_info_ptr->columns.size(); ++col_index) {
                verify(col_it != schema->end());
                verify(tb_info_ptr->columns[col_index].name == col_it->name);
                verify(tb_info_ptr->columns[col_index].type == col_it->type);

                if (tb_info_ptr->columns[col_index].is_primary) {
                    verify(col_it->indexed);
                    if (tb_info_ptr->tb_name == CHBENCHMARK_TB_CUSTOMER) { //XXX
                        if (col_it->name != "c_id") {
                            g_c_last_schema.add_column(col_it->name.c_str(), col_it->type, true);
                        }
                    } //XXX
                    if (tb_info_ptr->tb_name == CHBENCHMARK_TB_WAREHOUSE) {
                        if (tb_info_ptr->columns[col_index].name == "w_id") {
                            verify(w_id_cols.size() == 0);
//                            Log_info("Partition Col: %s", tb_info_ptr->columns[col_index].name.c_str());
                            w_id_cols.push_back(col_index);
                            break;
                        }
                    } else if (tb_info_ptr->columns[col_index].foreign != NULL) {
                        size_t fc_size = tb_info_ptr->columns[col_index].foreign->name.size();
                        std::string last4 = tb_info_ptr->columns[col_index].foreign->name.substr(fc_size - 4, 4);
                        if (last4 == "w_id") {
//                            Log_info("Partition Col: %s", tb_info_ptr->columns[col_index].name.c_str());
                            w_id_cols.push_back(col_index);
                        }
                    }
                }
                col_it++;
            }
            if (tb_info_ptr->tb_name == CHBENCHMARK_TB_CUSTOMER) { //XXX
                g_c_last_schema.add_column("c_id", mdb::Value::I32, true);
            } //XXX
            // Traverse the row and col to insert data
            for (row_index = 0; row_index < table.size(); ++row_index) {
                row_data.clear();
                for (col_index = 0; col_index < tb_info_ptr->columns.size(); ++col_index) {
//                    Log_info("Row index: %d, Col index: %d", row_index, col_index);
                    Value v_buf = value_parser(tb_info_ptr->columns[col_index].type, table[row_index][col_index]);
                    verified_success = true;
                    // verify the w_id
                    for (unsigned int w_id_col: w_id_cols) {
                        // if the col is w_id_col, it must equal to partition id
                        if (col_index == w_id_col) {
//                            Log_info("par_id = %d, v_buff = %s\n", par_id, table[row_index][col_index].c_str());
                            if (par_id != PartitionFromKey(v_buf, tb_info_ptr)) {
//                                Log_info("Not this partition, row_id: %s, should in %d", table[row_index][col_index].c_str(), PartitionFromKey(v_buf, tb_info_ptr));
                                verified_success = false;
                                break;
                            }
                        }
                    }
                    if (!verified_success) {
                        break;
                    }
                    // Record the foreign values
                    if (tb_info_ptr->columns[col_index].values != NULL) {
                        tb_info_ptr->columns[col_index].values->push_back(v_buf);
                    }
//                    if (tb_info_ptr->tb_name == CHBENCHMARK_TB_WAREHOUSE && tb_info_ptr->columns[col_index].is_primary) {
//                        Log_info ("WAREHOUSE SIZE: %d", tb_info_ptr->columns[col_index].values->size());
//                    }
//                    Log_info("Insert row: %s", table[row_index][col_index].c_str());
                    row_data.push_back(v_buf);
                }

                // insert data
                if (col_index == tb_info_ptr->columns.size()) {
                    cnt++;
                    mdb::Row *r = frame_->CreateRow(schema, row_data);
                    table_ptr->insert(r);

                    //
                    if (tbl_sec_ptr) {
                        rrr::i32 cur_o_id_buf = r->get_column("o_id").get_i32();
                        const mdb::Schema *sch_buf = tbl_sec_ptr->schema();
                        mdb::MultiBlob mb_buf(sch_buf->key_columns_id().size());
                        mdb::Schema::iterator col_info_it = sch_buf->begin();
                        size_t mb_i = 0;
                        for (; col_info_it != sch_buf->end(); col_info_it++)
                            if (col_info_it->indexed)
                                mb_buf[mb_i++] = r->get_blob(col_info_it->name);
                        mdb::SortedTable::Cursor rs = tbl_sec_ptr->query(mb_buf);
                        if (rs.has_next()) {
                            mdb::Row *r_buf = rs.next();
                            rrr::i32 o_id_buf = r_buf->get_column("o_id").get_i32();
                            if (o_id_buf < cur_o_id_buf)
                                r_buf->update("o_id", cur_o_id_buf);
                        } else {
                            std::vector <Value> sec_row_data_buf;
                            for (col_info_it = sch_buf->begin();
                                 col_info_it != sch_buf->end(); col_info_it++)
                                sec_row_data_buf.push_back(r->get_column(col_info_it->name));
                            mdb::Row *r_buf = frame_->CreateRow(sch_buf, sec_row_data_buf);
                            tbl_sec_ptr->insert(r_buf);
                        }
                    }


                    //XXX c_last secondary index
                    if (tb_info_ptr->tb_name == CHBENCHMARK_TB_CUSTOMER) {
                        std::string c_last_buf = r->get_column("c_last").get_str();
                        rrr::i32 c_id_buf = r->get_column("c_id").get_i32();
                        size_t mb_size = g_c_last_schema.key_columns_id().size(), mb_i = 0;
                        mdb::MultiBlob mb_buf(mb_size);
                        mdb::Schema::iterator col_info_it = g_c_last_schema.begin();
                        for (; col_info_it != g_c_last_schema.end(); col_info_it++) {
                            mb_buf[mb_i++] = r->get_blob(col_info_it->name);
                        }
                        g_c_last2id.insert(std::make_pair(c_last_id_t(c_last_buf, mb_buf, &g_c_last_schema), c_id_buf));
                    } //XXX
                }
            }
        }
//        Log_info("Populate Records Num: %d", cnt);
//        Log_info("key_columns_id size: %d", (int) g_c_last_schema.key_columns_id().size());
//        Log_info("Finish Populate: %s", tb_info_ptr->tb_name.c_str());
    }

    int ChBenchmarkSharding::PopulateTables(parid_t par_id) {
        bool populated;
        do {
            populated = false;
            for (auto tb_it = tb_infos_.begin(); tb_it != tb_infos_.end(); tb_it++) {

                tb_info_t *tb_info = &(tb_it->second);
//                Log_info("Here Table Name: %s, State: %d, Ready: %d", tb_it->first.c_str(), tb_info->populated[par_id], Ready2Populate(tb_info));
                verify(tb_it->first == tb_info->tb_name);

                // TODO is this unnecessary?
                auto it = tb_info->populated.find(par_id);
                if (it == tb_info->populated.end()) {
                    tb_info->populated[par_id] = false;
                }
                Log_info("Table: %s, done: %d, ready: %d\n", tb_info->tb_name.c_str(), tb_info->populated[par_id],Ready2Populate(tb_info));
                if (!tb_info->populated[par_id] &&
                    Ready2Populate(tb_info)) {
                    PopulateTable(tb_info, par_id);
                    tb_info->populated[par_id] = true;
                    populated = true;
                }
            }
        } while (populated);

        release_foreign_values();
        return 0;

        fprintf(stderr, "%s -- implement in subclass\n", __FUNCTION__);
        verify(0);
    }

} // namespace rococo