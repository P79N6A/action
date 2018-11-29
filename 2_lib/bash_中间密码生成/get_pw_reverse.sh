#!/bin/sh
read -p 'input passwd: ' pswd
midstr=`echo  $pswd | openssl md5 | awk '{print $2}'`
#echo $midstr
echo '$1='$1
echo $1  | openssl enc -e -base64 -aes-128-cbc -pass pass:$midstr
