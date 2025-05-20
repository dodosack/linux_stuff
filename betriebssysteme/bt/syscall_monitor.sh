#!/usr/bin/env bash
#
# syscall_monitor.sh — continuously monitor and nicely print syscall counts
#
# Usage: ./syscall_monitor.sh [-i INTERVAL] [-s SYSCALL1,SYSCALL2,…]
#

INTERVAL=2
SYSCALLS="openat,read,write,close"

usage(){
  awk 'NR q>1 && /^#/{ sub(/^# ?/,""); print }' "$0"
  exit 0
}

while getopts "hi:s:" opt; do
  case $opt in
    h) usage ;;
    i) INTERVAL=$OPTARG ;;
    s) SYSCALLS=$OPTARG ;;
    *) usage ;;
  esac
done

IFS=',' read -r -a ARR <<< "$SYSCALLS"
PROBES=""
for sc in "${ARR[@]}"; do
  PROBES+="tracepoint:syscalls:sys_enter_${sc},"
done
PROBES=${PROBES%,}

read -r -d '' BPF_SCRIPT <<EOF
$PROBES
{
  @counts[probe]++;
}
interval:s:$INTERVAL
{
  printf("\\n=== %s ===\\n", strftime("%Y-%m-%d %H:%M:%S", nsecs));
  printf("%-35s %s\\n", "SYSCALL", "COUNT");
  print(@counts);
  clear(@counts);
}
EOF

exec sudo bpftrace -e "$BPF_SCRIPT"
