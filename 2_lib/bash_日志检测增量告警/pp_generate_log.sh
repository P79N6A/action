#!/bin/sh
n=$(( (RANDOM % 3000 ) + 2000 ))
for ((i=0;i < $n;i++))
do
	((x=i%3))
	echo "["`date "+%Y%m%d %H:%M:%S"`"] $x ERROR aaa bbb 123 ccc ddd eee" >> log.error.test
done
