#!/bin/bash

# Execute the .out file

./test.out <<LOAD1 #region
1
2
LOAD1

./test.out <<TEST1
3
2
(r_name = 'EUROPE')
TEST1

./test.out <<TEST2
3
2
(r_name < 'middle east') AND (r_regionkey > 1)
TEST2

./test.out <<LOAD2 #nation
1
1
LOAD2

./test.out <<TEST3
3
1
(n_regionkey = 3) AND (n_nationkey > 10) AND (n_name > 'japan')
TEST3

./test.out <<LOAD3 #supplier
1
8
LOAD3

./test.out <<TEST4
3
8
(s_suppkey < 10)
TEST4

./test.out <<TEST5
3
8
(s_nationkey = 18) AND (s_acctbal > 1000.0) AND (s_suppkey < 400)
TEST5

./test.out <<LOAD4 #customer
1
3
LOAD4

./test.out <<TEST6
3
3
(c_nationkey = 23) AND (c_mktsegment = 'FURNITURE') AND (c_acctbal > 7023.99) AND (c_acctbal < 7110.83)
TEST6

./test.out <<LOAD5 #part
1
4
LOAD5

./test.out <<TEST7
3
4
(p_brand = 'Brand#13') AND (p_retailprice > 500.0) AND (p_retailprice < 930.0) AND (p_size > 28) AND (p_size < 1000000)
TEST7

./test.out <<LOAD6 #partsupp
1
5
LOAD6

./test.out <<TEST8
3
5
(ps_supplycost > 999.98)
TEST8

./test.out <<TEST9
3
5
(ps_availqty < 10) AND (ps_supplycost > 100.0) AND (ps_suppkey < 300)
TEST9

./test.out <<LOAD7 #orders
1
6
LOAD7

./test.out <<TEST10
3
6
(o_orderpriority = '1-URGENT') AND (o_orderstatus = '0') AND (o_shippriority = 0) AND (o_totalprice > 1015.68) AND (o_totalprice < 1051.89)
TEST10

./test.out <<LOAD8 #lineitem
1
7
LOAD8

./test.out <<TEST11
3
7
(l_shipdate > '1994-01-01') AND (l_shipdate < '1994-01-07') AND (l_discount > 0.05) AND (l_discount < 0.06) AND (l_quantity = 4.0) 
TEST11

./test.out <<TEST12
3
7
(l_orderkey > 100) AND (l_orderkey < 1000) AND (l_partkey > 100) AND (l_partkey < 5000) AND (l_shipmode = 'AIR') AND (l_linestatus = 'F') AND (l_tax < 0.07)
TEST12

echo -e "\nStarting Test for sort order [(n_regionkey)]" 
./test.out <<TEST13
5
1
16
(n_regionkey)
TEST13

echo -e "\nStarting Test for sort order [(c_phone) AND (c_custkey)]" 
./test.out <<TEST14
5
3
16
(c_phone) AND (c_custkey)
TEST14

echo -e "\nStarting Test for sort order [(o_totalprice) AND (o_custkey)]" 
./test.out <<TEST15
5
6
128
(o_totalprice) AND (o_custkey)
TEST15

echo -e "\nStarting Test for sort order [(n_regionkey) AND (n_name)]" 
./test.out <<TEST16
5
1
8
(n_regionkey) AND (n_name)
TEST16

echo -e "\nStarting Test for sort order [(n_name)]" 
./test.out <<TEST17
5
1
8
(n_name)
TEST17
#(n_regionkey) AND (n_name)