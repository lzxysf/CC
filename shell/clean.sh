#!/bin/sh

cd /

echo $1

$(ls $1)

clean() {
	for file in $(ls $1)
	do
		if [ -d $file ]
		then
			echo $file
		else
			echo "www"
		fi
	done
}
