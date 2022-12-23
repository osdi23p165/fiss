# ./run_all.py -hh config/out.yml -s '8'  -c '16' -r '3'  -cc config/tpcc.yml -cc config/client_closed.yml -cc config/concurrent_15.yml -cc config/tapir.yml -b tpcc -m tapir:tapir -t n_clients --allow-client-overlap testing1

# ./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3'  -cc config/tpcc_crt_ratio.yml -cc config/client_closed.yml -cc config/concurrent_15.yml -cc config/chronos.yml -b tpcc -m chronos:chronos -t crt_ratio --allow-client-overlap con15



# # # n_clients
# ./run_all.py -hh config/out.yml -s '8'  -c '8:25:8' -r '3'  -cc config/tpcc.yml -cc config/client_closed.yml -cc config/concurrent_15.yml -cc config/chronos.yml -b tpcc -m chronos:chronos -t n_clients --allow-client-overlap testing1
# ./run_all.py -hh config/out.yml -s '8'  -c '8:25:8' -r '3'  -cc config/tpcc.yml -cc config/client_closed.yml -cc config/concurrent_15.yml -cc config/slog.yml -b tpcc -m slog:slog -t n_clients --allow-client-overlap testing1
# ./run_all.py -hh config/out.yml -s '8'  -c '8:25:8' -r '3'  -cc config/tpcc.yml -cc config/client_closed.yml -cc config/concurrent_15.yml -cc config/brq.yml -b tpcc -m brq:brq -t n_clients --allow-client-overlap testing1
# ./run_all.py -hh config/out.yml -s '8'  -c '8:25:8' -r '3'  -cc config/tpcc.yml -cc config/client_closed.yml -cc config/concurrent_15.yml -cc config/tapir.yml -b tpcc -m tapir:tapir -t n_clients --allow-client-overlap testing1


# # crt ratio
# ./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3'  -cc config/tpcc_crt_ratio.yml -cc config/client_closed.yml -cc config/concurrent_15.yml -cc config/chronos.yml -b tpcc -m chronos:chronos -t crt_ratio --allow-client-overlap con15
# ./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3'  -cc config/tpcc_crt_ratio.yml -cc config/client_closed.yml -cc config/concurrent_25.yml -cc config/chronos.yml -b tpcc -m chronos:chronos -t crt_ratio --allow-client-overlap con25

# ./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3'  -cc config/tpcc_crt_ratio.yml -cc config/client_closed.yml -cc config/concurrent_15.yml -cc config/slog.yml -b tpcc -m slog:slog -t crt_ratio --allow-client-overlap con15
# ./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3'  -cc config/tpcc_crt_ratio.yml -cc config/client_closed.yml -cc config/concurrent_25.yml -cc config/slog.yml -b tpcc -m slog:slog -t crt_ratio --allow-client-overlap con25


# ./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3'  -cc config/tpcc_crt_ratio.yml -cc config/client_closed.yml -cc config/concurrent_15.yml -cc config/brq.yml -b tpcc -m brq:brq -t crt_ratio --allow-client-overlap con15
./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3'  -cc config/tpcc_crt_ratio.yml -cc config/client_closed.yml -cc config/concurrent_25.yml -cc config/brq.yml -b tpcc -m brq:brq -t crt_ratio --allow-client-overlap con25

./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3'  -cc config/tpcc_crt_ratio.yml -cc config/client_closed.yml -cc config/concurrent_15.yml -cc config/tapir.yml -b tpcc -m tapir:tapir -t crt_ratio --allow-client-overlap con15
./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3'  -cc config/tpcc_crt_ratio.yml -cc config/client_closed.yml -cc config/concurrent_25.yml -cc config/tapir.yml -b tpcc -m tapir:tapir -t crt_ratio --allow-client-overlap con25

# # # tpca
./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3' -z 0.5  -cc config/tpca_zipf.yml -cc config/client_closed.yml -cc config/concurrent_50.yml -cc config/chronos.yml -b tpca -t tpca -m chronos:chronos --allow-client-overlap con50 
./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3' -z 0.5  -cc config/tpca_zipf.yml -cc config/client_closed.yml -cc config/concurrent_75.yml -cc config/chronos.yml -b tpca -t tpca -m chronos:chronos --allow-client-overlap con75
./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3' -z 0.5  -cc config/tpca_zipf.yml -cc config/client_closed.yml -cc config/concurrent_100.yml -cc config/chronos.yml -b tpca -t tpca -m chronos:chronos --allow-client-overlap con100

./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3' -z 0.5  -cc config/tpca_zipf.yml -cc config/client_closed.yml -cc config/concurrent_50.yml -cc config/slog.yml -b tpca -t tpca -m slog:slog --allow-client-overlap con50
./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3' -z 0.5  -cc config/tpca_zipf.yml -cc config/client_closed.yml -cc config/concurrent_75.yml -cc config/slog.yml -b tpca -t tpca -m slog:slog --allow-client-overlap con75
./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3' -z 0.5  -cc config/tpca_zipf.yml -cc config/client_closed.yml -cc config/concurrent_100.yml -cc config/slog.yml -b tpca -t tpca -m slog:slog --allow-client-overlap con100

./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3' -z 0.5  -cc config/tpca_zipf.yml -cc config/client_closed.yml -cc config/concurrent_50.yml -cc config/brq.yml -b tpca -t tpca -m brq:brq --allow-client-overlap con50
./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3' -z 0.5  -cc config/tpca_zipf.yml -cc config/client_closed.yml -cc config/concurrent_75.yml -cc config/brq.yml -b tpca -t tpca -m brq:brq --allow-client-overlap con75
./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3' -z 0.5  -cc config/tpca_zipf.yml -cc config/client_closed.yml -cc config/concurrent_100.yml -cc config/brq.yml -b tpca -t tpca -m brq:brq --allow-client-overlap con100

./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3' -z 0.5  -cc config/tpca_zipf.yml -cc config/client_closed.yml -cc config/concurrent_50.yml -cc config/tapir.yml -b tpca -t tpca -m tapir:tapir --allow-client-overlap con50
./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3' -z 0.5  -cc config/tpca_zipf.yml -cc config/client_closed.yml -cc config/concurrent_50.yml -cc config/tapir.yml -b tpca -t tpca -m tapir:tapir --allow-client-overlap con75
./run_all.py -hh config/out.yml -s '8'  -c '8' -r '3' -z 0.5  -cc config/tpca_zipf.yml -cc config/client_closed.yml -cc config/concurrent_100.yml -cc config/tapir.yml -b tpca -t tpca -m tapir:tapir --allow-client-overlap con100