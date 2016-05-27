#!/bin/sh

iperf -s -p 98765 -t 10 & ../ScreamServer 56789 
