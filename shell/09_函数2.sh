#! /bin/sh

is_directory()
{
    DIR_NAME=$1
    if [ -d $DIR_NAME ]
    then return 0       #注意shell中0和1的逻辑是反的
    else return 1
    fi
}

if  is_directory "$1"   #注意此处没有[]
then echo "是一个目录"
fi
