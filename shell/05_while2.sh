#!/bin/sh

COUNTER=1
while [ "$COUNTER" -lt 10 ]
do
    echo "Here we go again"
    COUNTER=$(($COUNTER+1))
done
