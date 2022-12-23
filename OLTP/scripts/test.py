import subprocess
from exp_config import *


trial_name = "n_clients"

sysname = "brq"
cps = 10

cmd = ["./run_all.py"]
cmd.extend(["-hh", "/htap/OLTP/dast/config/out.yml"])
cmd.extend(["-s", str(N_REGIONS * N_SHARD_PER_REGION)])
cmd.extend(["-c", str(N_REGIONS * N_SHARD_PER_REGION * cps)])
cmd.extend(["-r", "3"])
cmd.extend(["-cc", "/htap/OLTP/dast/config/chBenchmark.yml"])
cmd.extend(["-cc", "/htap/OLTP/dast/config/client_closed.yml"])
cmd.extend(["-cc", "/htap/OLTP/dast/config/concurrent_1.yml"]) # this value will be changed anyway, use 1
cmd.extend(["-cc", "/htap/OLTP/dast/config/" + sysname + ".yml"])
cmd.extend(["-b", "ch_benchmark"])
cmd.extend(["-m", sysname + ":" + sysname])
cmd.extend(["-t", trial_name])
cmd.extend(["--allow-client-overlap"])
cmd.extend(["testing"])

cmd = [str(c) for c in cmd]

res = subprocess.call(cmd)



