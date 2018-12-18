#!/bin/sh
echo '------------------test -z'
if [ -z $nonexist ];then
	echo 'noexist'
fi
echo '------------------test -n'
a=''
if [ -n ];then
	echo 'no target is also OK';
fi
if [ -n "$a" ];then
	echo '$a='$a
fi
b=`echo $*|grep good`
if [ -n "$b" ];then
	echo '$b='$b
fi
