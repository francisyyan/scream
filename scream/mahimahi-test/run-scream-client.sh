#!/bin/sh

uplink=./Verizon-LTE-short.up
downlink=./Verizon-LTE-short.down
uplink_log=./uplink.log
cmd="../ScreamClient \$MAHIMAHI_BASE 12345"

mm-link $uplink $downlink --meter-uplink --once --uplink-log=$uplink_log -- sh -c "$cmd" 
