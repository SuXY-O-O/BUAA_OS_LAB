#!/bin/bash
#18373187 write for lab0

grep -n "$2" $1 | awk -F : '{print $1}' >$3 
