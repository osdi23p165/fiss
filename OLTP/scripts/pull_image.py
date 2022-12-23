from exp_config import *
import yaml
import subprocess

hosts = [] 
with open (HOSTS_FILE, 'r') as f:
    data = yaml.load(f)
    hosts = data['hosts']

for h in hosts:
    ssh_command = "docker pull " +  IMAGE_NAME 
    print(ssh_command)
    subprocess.call(['ssh', h, ssh_command])

