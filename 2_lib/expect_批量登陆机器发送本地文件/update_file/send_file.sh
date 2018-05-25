#!/bin/sh
file=$1
dst_host=$2
dst_path=$3

declare -A hm
mapfile tg < machine.txt
for ((i=0; i<${#tg[*]}; i++ )) ; do
	t=(${tg[i]})
#	echo ${t[0]}
#	echo ${t[1]}
	hm["${t[0]}"]=${t[1]}
done
	
expect send_file.expect $dst_host ${hm[$dst_host]} $file $dst_path
