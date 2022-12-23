from exp_config import *
import yaml
import subprocess

hosts = [] 
with open (HOSTS_FILE, 'r') as f:
    data = yaml.load(f)
    hosts = data['hosts']

for h in hosts:
    print("stopping container on " + h)
    ssh_command = "docker stop $(docker ps -q --filter network=" +  NETWORK_NAME + ")"
    print(ssh_command)
    subprocess.call(['ssh', h, ssh_command])

for h in hosts:
    print("stopping container on " + h)
    ssh_command = "docker rm $(docker ps -aq --filter network=" +  NETWORK_NAME + ")"
    print(ssh_command)
    subprocess.call(['ssh', h, ssh_command])
