#!/bin/sh

if [ -f /bin/bash ]
then echo "/bin/bash is a file"
else echo "/bin/bash is NOT a file"
fi

# :是一个特殊的命令称为空命令，该命令不做任何事，但Exit Status总为真。
if :
then echo "always true"
fi

# read命令的作用是等待用户输入一行字符串，然后存入一个Shell变量中
echo "Is it morning? Please answer yes or no."
read YES_OR_NO
if [ "$YES_OR_NO" = "yes" ]
then echo "Good morning!"
elif [ "$YES_OR_NO" = "no" ]
then echo "Good afternoon!"
else 
echo "Sorry, $YES_OR_NO not recognized. Enter yes or no"
exit 1
fi

# 判断当前用户是不是root
if [ "$(whoami)" = "root" ]
then echo "power"
else echo "weak"
fi

exit 0
