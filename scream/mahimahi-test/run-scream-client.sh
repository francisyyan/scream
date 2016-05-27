#!/bin/sh

uplink=1Mbps-trace
downlink=1Mbps-trace
uplink_log=uplink.log
cmd="../ScreamClient \$MAHIMAHI_BASE 12345"

mm-link $uplink $downlink --once --uplink-log=$uplink_log -- sh -c "$cmd" 
