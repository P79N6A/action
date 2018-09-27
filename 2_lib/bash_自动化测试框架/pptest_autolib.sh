#!/bin/sh
result_file="result_of_$0"
cur_seq=0;
function assertEqual
{
	if [ $1 == $2 ];then
	re="succ";
	else
	re="fail";
	fi
	echo "[${cur_seq}] ${FUNCNAME[1]}=>"$re"($1=$2) $3" >> $result_file
}
function assertNotEqual
{
	if [ $1 != $2 ];then
	re="succ";
	else
	re="fail";
	fi
	echo "[${cur_seq}] ${FUNCNAME[1]}=>"$re"($1!=$2) $3" >> $result_file
}
function assertLessEqual
{
	if [ $1 -le $2 ];then
	re="succ";
	else
	re="fail";
	fi
	echo "[${cur_seq}] ${FUNCNAME[1]}=>"$re"($1<=$2) $3" >> $result_file
}
function assertLess
{
	if [ $1 -lt $2 ];then
	re="succ";
	else
	re="fail";
	fi
	echo "[${cur_seq}] ${FUNCNAME[1]}=>"$re"($1<$2) $3" >> $result_file
}
function clear_result
{
	rm -f $result_file
}
function output_result
{
	h='############################################'`date` 
	sed -i "1i $h" $result_file
	cat $result_file |  sed $'s/succ/\e[1m&\e[0m/' | sed $'s/fail/\e[41m&\e[0m/' 
}

function rpl_json 
{
	sed -i '/'$2'/{s@\(^[^:]*:"*\)[^",]*\(.*$\)@\1'$3'\2@g}' $1
}

#if [ "$(type -t setUp)" == 'function' ];then
#echo 'yesyesyeysesyeysy'
#fi
#exit 0;


clear_result;

all=(`cat $0 | grep '^test_[a-zA-Z0-9_()]*[ \t]*$'|tr '\n' ' '`)
echo "read file is $0";
for ((i=0;i<${#all[*]};i++))
do
all[$i]=`echo "${all[$i]}" | sed 's/^.*\(test_[a-zA-Z0-9_]*\).*$/\1/g'`
echo "$i=>"${all[$i]}
done

if [ $# == 0 ];then 
exit 0;
fi
if [ $1 == 'all' ];then
	setUp;
	for ((i=0;i<${#all[*]};i++))
	do
	echo "====================================>$i/${#all[*]}"
	cur_seq=$i;
	stepSetUp;
	${all[$i]}
	stepTearDown;
	done
	tearDown;
else
	setUp;
	for i in $@
	do
	echo "====================================>$i/${#all[*]}"
	cur_seq=$i;
	stepSetUp
	${all[$i]}
	stepTearDown
	done
	tearDown;
fi

output_result;
