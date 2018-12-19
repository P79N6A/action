#!/bin/sh
pid=`ps aux | grep aim.out|grep -v grep | awk '{print $2}'`
echo 'pid is '$pid
kill -9 $pid
./aim.out &
pid=`ps aux | grep aim.out|grep -v grep | awk '{print $2}'`
echo 'new pid is '$pid
#./tool.out  $pid 0x600e98 1

