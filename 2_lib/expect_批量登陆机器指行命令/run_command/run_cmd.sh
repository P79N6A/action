#!/bin/sh
if [ $# != 1 ];then
echo 'wrong argument';
exit 0;
fi
cf=`cat $1|tr '\n' ' '`
declare -A hm
mapfile tg < machine.txt
for ((i=0; i<${#tg[*]}; i++ )) ; do
	t=(${tg[i]})
#	echo ${t[0]}
#	echo ${t[1]}
	hm["${t[0]}"]=${t[1]}
	expect command.expect ${t[0]} ${t[1]} "$cf"
done
	
