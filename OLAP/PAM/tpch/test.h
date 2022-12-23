//
// Created by 17124 on 2021/8/11.
//
#pragma once
#ifndef PARTIAL_HTAP_TEST_H
#define PARTIAL_HTAP_TEST_H

#include <vector>
#include <map>
#include <string.h>

struct insert_form {
    std::string col_name;
    std::string value;
};

void Vec2Map(std::vector <insert_form> v, std::map<std::string, std::string> &row_map);
double insertOrder(int64_t txn_id, std::vector <insert_form> v);
double insertOrderline(int64_t txn_id, std::vector<insert_form> v);
double updateMap(std::string table_name, std::string pkeyColName, std::string pkeyVal, std::string colId, std::string value);
double op(int op_type, int64_t txnid, std::string table_name, std::string pkey_col_name, std::string pkey_val, std::string col_name, std::string value);
void statistic(int interval);

std::pair<double, double> geo_mean(double *x, int round);
std::pair<double, double> arith_mean(double *x, int round);

void output_res(double **tm, int round, int queries);

void importCSV(std::string data_directory, bool verbose = false);
void run_query(int q, int round = 6, bool verbose = false, bool query_time = false);

#endif //PARTIAL_HTAP_TEST_H
