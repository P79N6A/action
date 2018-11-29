#!/bin/sh
read -p 'input passwd: ' pswd
midstr=`echo  $pswd | openssl md5 | awk '{print $2}'`
#echo $midstr
declare -A hm=(
['127.0.0.1']='U2FsdGVkX1/+WtHSJ+ksvzUMFEF4CWfOLy7+UdtmrQ8='
)
for k in ${!hm[@]}  
do  
#    	echo ${hm[$k]}  
	rwd=`echo ${hm[$k]}  | openssl enc -d -base64 -aes-128-cbc -pass pass:$midstr`
	hm[$k]=$rwd;
	#echo ${hm[$k]};
done  
