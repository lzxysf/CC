#!/bin/sh
COUNT=0
while :
do
    COUNT=$(($COUNT+1))
    if [ "$COUNT" -gt 5 ]
    then echo "超出次数限制";break
    fi
    read PASSWD
    if [ "$PASSWD" = "123456" ]
    then echo "密码正确";break
    else continue
    fi
done