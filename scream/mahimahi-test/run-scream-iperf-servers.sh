#!/bin/sh

iperf -s -p 98765 -t 3 & ../ScreamServer 56789 
