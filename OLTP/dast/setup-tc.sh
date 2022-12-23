#!/bin/bash

# ./setup-tc.sh eth0 master

echo "$1 $2"

device=$1
master_ip=$(getent hosts $2|awk '{print $1}')
network_str=$(ifconfig $device|grep 'inet addr'|tr -s ' '| cut -d' ' -f3|cut -d':' -f2)
network=$(echo $network_str|cut -d'.' -f1).$(echo $network_str|cut -d'.' -f2).0.0

echo "master ip @ $master_ip, network @ $network"
tc qdisc del dev $device root
tc qdisc add dev $device root handle 1: prio bands 3 priomap 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
tc qdisc add dev $device parent 1:2 handle 2: netem delay 50ms
tc filter add dev $device parent 1: protocol ip pref 1 u32 match ip dst $master_ip flowid 1:3
tc filter add dev $device parent 1: protocol ip pref 2 u32 match ip dst $network/16 flowid 1:2
