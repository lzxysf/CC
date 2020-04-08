#!/bin/sh

clean() {
	cd $1
	for file in $(ls)
	do
		if [ -d $file ]
		then (clean $file)
		else 
			if [ $file = "a.out" ]
			then rm a.out;echo $(pwd)/$file
			fi
		fi
	done
}

clean /home/ubuntu/CC
