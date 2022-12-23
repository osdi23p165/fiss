./run_all.py -hh config/edge-seperate.yml -s '4' -c '4' -r '3' -cc config/tpcch-xs.yml -cc config/client_closed.yml -cc config/concurrent_10.yml -cc config/tapir.yml -b tpcc -m tapir:tapir --allow-client-overlap testing01

sleep 5s

./run_all.py -hh config/edge-seperate.yml -s '4' -c '4' -r '3' -cc config/tpcch-xs.yml -cc config/client_closed.yml -cc config/concurrent_100.yml -cc config/tapir.yml -b tpcc -m tapir:tapir --allow-client-overlap testing02

sleep 5s

./run_all.py -hh config/edge-seperate.yml -s '4' -c '4' -r '3' -cc config/tpcch-xs.yml -cc config/client_closed.yml -cc config/concurrent_200.yml -cc config/tapir.yml -b tpcc -m tapir:tapir --allow-client-overlap testing03

sleep 5s

./run_all.py -hh config/edge-seperate.yml -s '4' -c '4' -r '3' -cc config/tpcch-xs.yml -cc config/client_closed.yml -cc config/concurrent_300.yml -cc config/tapir.yml -b tpcc -m tapir:tapir --allow-client-overlap testing04

sleep 5s

./run_all.py -hh config/edge-seperate.yml -s '4' -c '4' -r '3' -cc config/tpcch-xs.yml -cc config/client_closed.yml -cc config/concurrent_400.yml -cc config/tapir.yml -b tpcc -m tapir:tapir --allow-client-overlap testing05

sleep 5s

./run_all.py -hh config/edge-seperate.yml -s '4' -c '4' -r '3' -cc config/tpcch-xs.yml -cc config/client_closed.yml -cc config/concurrent_500.yml -cc config/tapir.yml -b tpcc -m tapir:tapir --allow-client-overlap testing06

sleep 5s

./run_all.py -hh config/edge-seperate.yml -s '4' -c '4' -r '3' -cc config/tpcch-xs.yml -cc config/client_closed.yml -cc config/concurrent_1000.yml -cc config/tapir.yml -b tpcc -m tapir:tapir --allow-client-overlap testing07

sleep 5s

./run_all.py -hh config/edge-seperate.yml -s '4' -c '4' -r '3' -cc config/tpcch-xs.yml -cc config/client_closed.yml -cc config/concurrent_2000.yml -cc config/tapir.yml -b tpcc -m tapir:tapir --allow-client-overlap testing08

sleep 5s

./run_all.py -hh config/edge-seperate.yml -s '4' -c '4' -r '3' -cc config/tpcch-xs.yml -cc config/client_closed.yml -cc config/concurrent_3000.yml -cc config/tapir.yml -b tpcc -m tapir:tapir --allow-client-overlap testing09

sleep 5s

./run_all.py -hh config/edge-seperate.yml -s '4' -c '4' -r '3' -cc config/tpcch-xs.yml -cc config/client_closed.yml -cc config/concurrent_5000.yml -cc config/tapir.yml -b tpcc -m tapir:tapir --allow-client-overlap testing10

sleep 5s

./run_all.py -hh config/edge-seperate.yml -s '4' -c '4' -r '3' -cc config/tpcch-xs.yml -cc config/client_closed.yml -cc config/concurrent_10000.yml -cc config/tapir.yml -b tpcc -m tapir:tapir --allow-client-overlap testing11
