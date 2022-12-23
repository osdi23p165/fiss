#!/bin/bash
for filename in log/proc-S*.log; do
  N=`cat $filename | grep logg | cut -d '=' -f 2 | wc -l`
  echo -n "${filename}: "
  let "TRUNC = $N * 2 / 10"
  echo "$N results"
  cat $filename | grep logg | cut -d '=' -f 2 | head -n -$TRUNC | tail -n +$TRUNC > "log/$(basename "$filename" .log).rtt"
done
cat log/*.rtt | st

NTotal=`cat log/*.rtt | wc -l`
let "Med= $NTotal / 2"
echo -n "Median latency: "
cat log/*.rtt | sort -n | sed -e "${Med}q;d"

let "Tail95= $NTotal * 95 / 100"
echo -n "95 percentail latency: "
cat log/*.rtt | sort -n | sed -e "${Tail95}q;d"

let "Tail99= $NTotal * 99 / 100"
echo -n "99 percentail latency: "
cat log/*.rtt | sort -n | sed -e "${Tail99}q;d"

let "Tail999= $NTotal * 999 / 1000"
echo -n "99.9 percentail latency: "
cat log/*.rtt | sort -n | sed -e "${Tail999}q;d"
