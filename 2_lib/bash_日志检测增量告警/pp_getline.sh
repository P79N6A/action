#!/bin/sh
#if has no new data in the file, this script will lock
ta=$1
tb=$2

while read line
do
#	echo "$line"
	t0=$line
	#avoid line that's not started with time string
	reg='^\[[0-9]{8}.*'
	if [[ "$t0" =~ $reg ]];then
		echo $t0 >> p_tmp_judged_line.log
		t1=${t0:1:17}
		t2=`date -d "$t1" +%s`
		if [ -z $ta ] || [ -z $t2 ] || [ -z $tb ];then
			echo 'cal time error';
			exit 0;
		fi
	
		if [ $ta -lt $t2 ] && [ $t2 -lt $tb ];then
			echo $t0;
		fi
	fi
		
done  
