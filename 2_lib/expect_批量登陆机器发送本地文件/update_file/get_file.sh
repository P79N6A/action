#!/bin/sh
src_host=$1
src_file=$2

declare -A hm
mapfile tg < machine.txt
for ((i=0; i<${#tg[*]}; i++ )) ; do
	t=(${tg[i]})
#	echo ${t[0]}
#	echo ${t[1]}
	hm["${t[0]}"]=${t[1]}
done
	
expect get_file.expect $src_host ${hm[$src_host]} $src_file
