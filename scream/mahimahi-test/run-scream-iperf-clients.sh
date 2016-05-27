#!/bin/sh

bottleneck=1Mbps-trace
uplink_log=uplink.log
cmd="(sleep 15; iperf -c \$MAHIMAHI_BASE -p 98765 -t 3) & ../ScreamClient \$MAHIMAHI_BASE 56789"

mm-link $bottleneck $bottleneck --once -- sh -c "$cmd" 
