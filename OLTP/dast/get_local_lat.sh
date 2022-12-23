#!/bin/bash
for filename in log/proc-C*.log; do
  N=`cat $filename | grep Local | grep Laten | cut -d '=' -f 2 | wc -l`
  echo -n "${filename}: "
  let "TRUNC = $N * 2 / 10"
  let "HALF = $N / 2"
  echo "$N results"
  cat $filename | grep Local | grep Laten | cut -d '=' -f 2 | head -n $HALF | tail -n +$TRUNC > "log/$(basename "$filename" .log).locallat"
done
cat log/*.locallat | st

NTotal=`cat log/*.locallat | wc -l`
let "Med= $NTotal / 2"
echo -n "Median latency: "
cat log/*.locallat | sort -n | sed -e "${Med}q;d"

#let "Tail75= $NTotal * 75 / 100"
#echo -n "75 percentail latency: "
#cat log/*.locallat | sort -n | sed -e "${Tail75}q;d"

let "Tail95= $NTotal * 95 / 100"
echo -n "95 percentail latency: "
cat log/*.locallat | sort -n | sed -e "${Tail95}q;d"

let "Tail99= $NTotal * 99 / 100"
echo -n "99 percentail latency: "
cat log/*.locallat | sort -n | sed -e "${Tail99}q;d"

let "Tail999= $NTotal * 999 / 1000"
echo -n "99.9 percentail latency: "
cat log/*.locallat | sort -n | sed -e "${Tail999}q;d"
