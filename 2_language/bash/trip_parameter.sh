#!/bin/sh
if [ $# != 1 ] ;then
	echo 'the script has one parameter';
	exit 0;
fi
#in linux 3.0 the folwing too way are different, the first way is illegal
if [ $1 = 1 ] || [ $1 = 'string' ];then
	echo $1;
fi

if [ $1 = 'string' ] || [ $1 = 1 ];then
	echo $1;
fi

