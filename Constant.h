//=============Updates in TPCC===================== 
//Piece is partitioned by column. 

// ======= New order txn =======
#define TPCC_NEW_ORDER              10
#define TPCC_NEW_ORDER_NAME         "NEW_ORDER"
//update TPCC_COL_DISTRICT_D_NEXT_O_ID
#define TPCC_NEW_ORDER_0            1000
//insert order - o_d_id 
#define TPCC_NEW_ORDER_1            1001
//insert order - o_w_id 
#define TPCC_NEW_ORDER_2            1002
//insert order - o_id 
#define TPCC_NEW_ORDER_3            1003
//insert order - o_c_id 
#define TPCC_NEW_ORDER_4            1004
//insert order - o_entry_d
#define TPCC_NEW_ORDER_5            1005
//insert order - o_carrier_id 
#define TPCC_NEW_ORDER_6            1006
//insert order - o_ol_cnt 
#define TPCC_NEW_ORDER_7            1007
//insert order - o_all_local 
#define TPCC_NEW_ORDER_8            1008
//insert Neworder - no_d_id
#define TPCC_NEW_ORDER_9            1009
//insert Neworder - no_w_id
#define TPCC_NEW_ORDER_10            1010
//insert Neworder - no_o_id
#define TPCC_NEW_ORDER_11            1011
//write stock - s_quantity
#define TPCC_NEW_ORDER_WS_0(i)           (10120000+i)
//write stock - s_ytd
#define TPCC_NEW_ORDER_WS_1(i)           (10121000+i)
//write stock - s_order_cnt
#define TPCC_NEW_ORDER_WS_2(i)           (10122000+i)
//write stock - s_remote_cnt
#define TPCC_NEW_ORDER_WS_3(i)           (10123000+i)
//insert orderline - ol_d_id
#define TPCC_NEW_ORDER_WOL_0(i)          (10130000+i)
//insert orderline - ol_w_id
#define TPCC_NEW_ORDER_WOL_1(i)          (10131000+i)
//insert orderline - ol_o_id
#define TPCC_NEW_ORDER_WOL_2(i)          (10132000+i)
//insert orderline - ol_number
#define TPCC_NEW_ORDER_WOL_3(i)          (10133000+i)
//insert orderline - ol_i_id
#define TPCC_NEW_ORDER_WOL_4(i)          (10134000+i)
//insert orderline - ol_supply_w_id
#define TPCC_NEW_ORDER_WOL_5(i)          (10135000+i)
//insert orderline - ol_delivery_d
#define TPCC_NEW_ORDER_WOL_6(i)          (10136000+i)
//insert orderline - ol_quantity
#define TPCC_NEW_ORDER_WOL_7(i)          (10137000+i)
//insert orderline - ol_amount
#define TPCC_NEW_ORDER_WOL_8(i)          (10138000+i)
//insert orderline - ol_dist_info
#define TPCC_NEW_ORDER_WOL_9(i)          (10139000+i)


// ======== Payment txn ========
#define TPCC_PAYMENT                20
#define TPCC_PAYMENT_NAME           "PAYMENT"
//write warehous - w_ytd seems skiped in the Janus codebase
#define TPCC_PAYMENT_0              2000
//write district - d_ytd 
#define TPCC_PAYMENT_1              2001
//write customer - c_balance 
#define TPCC_PAYMENT_2              2002
//write customer - c_ytd
#define TPCC_PAYMENT_3              2003
//write customer - c_payment
#define TPCC_PAYMENT_4              2004
//write customer - c_data (used only when credit is bad, just skip is no update)
#define TPCC_PAYMENT_5              2005
//insert history - h_key
#define TPCC_PAYMENT_6              2006
//insert history - h_c_id
#define TPCC_PAYMENT_7              2007
//insert history - h_c_d_id
#define TPCC_PAYMENT_8              2008
//insert history - h_c_w_id
#define TPCC_PAYMENT_9              2009
//insert history - h_d_id
#define TPCC_PAYMENT_10             2010
//insert history - h_w_id
#define TPCC_PAYMENT_11             2011
//insert history - h_date
#define TPCC_PAYMENT_12             2012
//insert history - h_amount
#define TPCC_PAYMENT_13             2013
//insert history - h_data
#define TPCC_PAYMENT_14             2014




// ===== Order status txn ======
//Read only
#define TPCC_ORDER_STATUS           30
#define TPCC_ORDER_STATUS_NAME      "ORDER_STATUS"


// ======= Delivery txn ========
#define TPCC_DELIVERY               40
#define TPCC_DELIVERY_NAME          "DELIVERY"
//delete neworder the min no_id row, seems skiped in the Janus codebase
//delete neworder - no_d_id
#define TPCC_DELIVERY_0             4000
//delete neworder - no_w_id
#define TPCC_DELIVERY_1             4001
//delete neworder - no_o_id
#define TPCC_DELIVERY_2             4002
//write order - o_carrier_id
#define TPCC_DELIVERY_3             4003
//write orderline - ol_delivery_d
#define TPCC_DELIVERY_4             4004
//write customer - c_balance
#define TPCC_DELIVERY_5             4005
//write customer - c_delivery_cnt
#define TPCC_DELIVERY_6             4006



#define TPCC_DELIVERY_3             4003

// ====== Stock level txn ======
//Read only
#define TPCC_STOCK_LEVEL            50
#define TPCC_STOCK_LEVEL_NAME       "STOCK_LEVEL"


// ===================== Query======================

// ====== Query1 ======
// select   ol_number,
// 	 sum(ol_quantity) as sum_qty,
// 	 sum(ol_amount) as sum_amount,
// 	 avg(ol_quantity) as avg_qty,
// 	 avg(ol_amount) as avg_amount,
// 	 count(*) as count_order
// from	 orderline
// where	 ol_delivery_d > '2007-01-02 00:00:00.000000'
// group by ol_number order by ol_number
//orderline - ol_number
#define TPCH_QUERY1_0           610000
//orderline - ol_quantity
#define TPCH_QUERY1_1           610001
//orderline - ol_amount
#define TPCH_QUERY1_2           610002
//orderline - ol_quantity
#define TPCH_QUERY1_3           610003
//orderline - ol_amount
#define TPCH_QUERY1_4           610004
//orderline - ol_delivery_d
#define TPCH_QUERY1_5           610005

// ====== Query2 ======
// select 	 su_suppkey, su_name, n_name, i_id, i_name, su_address, su_phone, su_comment
// from	 item, supplier, stock, nation, region,
// 	 (select s_i_id as m_i_id,
// 		 min(s_quantity) as m_s_quantity
// 	 from	 stock, supplier, nation, region
// 	 where	 mod((s_w_id*s_i_id),10000)=su_suppkey
// 	 	 and su_nationkey=n_nationkey
// 	 	 and n_regionkey=r_regionkey
// 	 	 and r_name like 'Europ%'
// 	 group by s_i_id) m
// where 	 i_id = s_i_id
// 	 and mod((s_w_id * s_i_id), 10000) = su_suppkey
// 	 and su_nationkey = n_nationkey
// 	 and n_regionkey = r_regionkey
// 	 and i_data like '%b'
// 	 and r_name like 'Europ%'
// 	 and i_id=m_i_id
// 	 and s_quantity = m_s_quantity
// order by n_name, su_name, i_id
//item - i_id
#define TPCH_QUERY2_0           620000
//item - i_name
#define TPCH_QUERY2_1           620001
//item - i_data
#define TPCH_QUERY2_2           620002
//supplier - su_suppkey
#define TPCH_QUERY2_3           620003
//supplier - su_name
#define TPCH_QUERY2_4           620004
//supplier - su_address
#define TPCH_QUERY2_5           620005
//supplier - su_phone
#define TPCH_QUERY2_6           620006
//supplier - su_comment
#define TPCH_QUERY2_7           620007
//supplier - su_nationkey
#define TPCH_QUERY2_8           620008
//stock - s_i_id
#define TPCH_QUERY2_9           620009
//stock - s_quantity
#define TPCH_QUERY2_10          620010
//nation - n_name
#define TPCH_QUERY2_11          620011
//nation - n_nationkey
#define TPCH_QUERY2_12          620012
//nation - n_regionkey
#define TPCH_QUERY2_13          620013
//region - r_name
#define TPCH_QUERY2_14          620014
//region - r_regionkey
#define TPCH_QUERY2_15          620015

// ====== Query3 ======
// select   ol_o_id, ol_w_id, ol_d_id,
// 	 sum(ol_amount) as revenue, o_entry_d
// from 	 customer, neworder, orders, orderline
// where 	c_state like 'A%'
// 	 and c_id = o_c_id
// 	 and c_w_id = o_w_id
// 	 and c_d_id = o_d_id
// 	 and no_w_id = o_w_id
// 	 and no_d_id = o_d_id
// 	 and no_o_id = o_id
// 	 and ol_w_id = o_w_id
// 	 and ol_d_id = o_d_id
// 	 and ol_o_id = o_id
// 	 and o_entry_d > '2007-01-02 00:00:00.000000'
// group by ol_o_id, ol_w_id, ol_d_id, o_entry_d
// order by revenue desc, o_entry_d
//customer - c_id
#define TPCH_QUERY3_0           630000
//customer - c_w_id
#define TPCH_QUERY3_1           630001
//customer - c_d_id
#define TPCH_QUERY3_2           630002
//customer - c_state
#define TPCH_QUERY3_3           630003
//neworder - no_w_id
#define TPCH_QUERY3_4           630004
//neworder - no_d_id
#define TPCH_QUERY3_5           630005
//neworder - no_o_id
#define TPCH_QUERY3_6           630006
//orders - o_entry_d
#define TPCH_QUERY3_7           630007
//orders - o_c_id
#define TPCH_QUERY3_8           630008
//orders - o_w_id
#define TPCH_QUERY3_9           630009
//orders - o_d_id
#define TPCH_QUERY3_10          630010
//orders - o_id
#define TPCH_QUERY3_11          630011
//orderline - ol_o_id
#define TPCH_QUERY3_12          630012
//orderline - ol_w_id
#define TPCH_QUERY3_13          630013
//orderline - ol_d_id
#define TPCH_QUERY3_14          630014

// ====== Query4 ======
// select	o_ol_cnt, count(*) as order_count
// from	orders
// where	o_entry_d >= '2007-01-02 00:00:00.000000'
// 	and o_entry_d < '2012-01-02 00:00:00.000000'
// 	and exists (select *
// 		    from orderline
// 		    where o_id = ol_o_id
// 		    and o_w_id = ol_w_id
// 		    and o_d_id = ol_d_id
// 		    and ol_delivery_d >= o_entry_d)
// group	by o_ol_cnt
// order	by o_ol_cnt
//orders - o_ol_cnt
#define TPCH_QUERY4_0           640000
//orders - o_entry_d
#define TPCH_QUERY4_1           640001
//orders - o_id
#define TPCH_QUERY4_2           640002
//orders - o_w_id
#define TPCH_QUERY4_3           640003
//orders - o_d_id
#define TPCH_QUERY4_4           640004
//orderline - ol_o_id
#define TPCH_QUERY4_5           640005
//orderline - ol_w_id
#define TPCH_QUERY4_6           640006
//orderline - ol_d_id
#define TPCH_QUERY4_7           640007
//orderline - ol_delivery_id
#define TPCH_QUERY4_8           640008

// ====== Query5 ======
// select	 n_name,
// 	 sum(ol_amount) as revenue
// from	 customer, orders, orderline, stock, supplier, nation, region
// where	 c_id = o_c_id
// 	 and c_w_id = o_w_id
// 	 and c_d_id = o_d_id
// 	 and ol_o_id = o_id
// 	 and ol_w_id = o_w_id
// 	 and ol_d_id=o_d_id
// 	 and ol_w_id = s_w_id
// 	 and ol_i_id = s_i_id
// 	 and mod((s_w_id * s_i_id),10000) = su_suppkey
// 	 and ascii(substr(c_state,1,1)) = su_nationkey
// 	 and su_nationkey = n_nationkey
// 	 and n_regionkey = r_regionkey
// 	 and r_name = 'Europe'
// 	 and o_entry_d >= '2007-01-02 00:00:00.000000'
// group by n_name
// order by revenue desc
//customer - c_id
#define TPCH_QUERY5_0           650000
//customer - c_w_id
#define TPCH_QUERY5_1           650001
//customer - c_d_id
#define TPCH_QUERY5_2           650002
//customer - c_state
#define TPCH_QUERY5_3           650003
//orders - o_w_id
#define TPCH_QUERY5_4           650004
//orders - o_c_id 
#define TPCH_QUERY5_5           650005
//orders - o_d_id
#define TPCH_QUERY5_6           650006
//orders - o_entry_d
#define TPCH_QUERY5_7           650007
//orders - o_id
#define TPCH_QUERY5_8           650008
//orderline - ol_o_id
#define TPCH_QUERY5_9           650009
//orderline - ol_w_id
#define TPCH_QUERY5_10          650010
//orderline - ol_d_id
#define TPCH_QUERY5_11          650011
//orderline - ol_i_id
#define TPCH_QUERY5_12          650012
//orderline - ol_amount 
#define TPCH_QUERY5_13          650013
//stock - s_w_id
#define TPCH_QUERY5_14          650014
//stock - s_i_id
#define TPCH_QUERY5_15          650015
//supplier - su_suppkey
#define TPCH_QUERY5_16          650016
//supplier - su_nationkey
#define TPCH_QUERY5_17          650017
//nation - n_name
#define TPCH_QUERY5_18          650018
//nation - n_nationkey
#define TPCH_QUERY5_19          650019
//nation - n_regionkey
#define TPCH_QUERY5_20          650020
//region - r_name
#define TPCH_QUERY5_21          650021
//region - r_regionkey
#define TPCH_QUERY5_22          650022

// ====== Query6 ======
// select	sum(ol_amount) as revenue
// from	orderline
// where	ol_delivery_d >= '1999-01-01 00:00:00.000000'
// 	and ol_delivery_d < '2020-01-01 00:00:00.000000'
// 	and ol_quantity between 1 and 100000
//orderline - ol_amount
#define TPCH_QUERY6_0           660000
//orderline - ol_delivery_d
#define TPCH_QUERY6_1           660001
//orderline - ol_qunantity
#define TPCH_QUERY6_2           660002


