#!/bin/bash
for filename in log/proc-C*.log; do
  N=`cat $filename | grep Dist | grep Laten | cut -d '=' -f 2 | wc -l`
  echo -n "${filename}: "
  let "TRUNC = $N * 3 / 10"
  let "HALF = $N * 7 / 10"
  echo "$N results"
  cat $filename | grep Dist | grep Laten | cut -d '=' -f 2 | head -n $HALF | tail -n +$TRUNC > "log/$(basename "$filename" .log).distlat"
done
cat log/*.distlat | st

NTotal=`cat log/*.distlat | wc -l`

let "Med= $NTotal / 2"
echo -n "Median latency: "
cat log/*.distlat | sort -n | sed -e "${Med}q;d"


let "Tail95= $NTotal * 95 / 100"
echo -n "95 percentail latency: "
cat log/*.distlat | sort -n | sed -e "${Tail95}q;d"

let "Tail99= $NTotal * 99 / 100"
echo -n "99 percentail latency: "
cat log/*.distlat | sort -n | sed -e "${Tail99}q;d"



let "Tail999= $NTotal * 999 / 1000"
echo -n "99.9 percentail latency: "
cat log/*.distlat | sort -n | sed -e "${Tail999}q;d"

