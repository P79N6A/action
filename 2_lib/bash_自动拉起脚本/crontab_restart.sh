#!/bin/sh
function send_alarm()
{
        if [ $# != 1 ];then
                echo "need msg"
        fi
        local time=`date`
        local sip=`/sbin/ifconfig | grep 'Bcast' | awk '{print $2}'`
        send_elf  "Time:$time $sip $1"
}
function check_log()
{
        if [ $# != 1 ];then
                echo "need path"
        fi
        local cur_path=`pwd`
        local path=$1
        cd $path

        local file="file_size_used_by_crontab"
        if [ ! -f "$file" ];then
                local size=`du --max-depth=0 | awk '{print $1}'`
                echo "$size" > $file;
        fi
        #exit
        local last_size=`cat $file`
        local now_size=`du --max-depth=0 | awk '{print $1}'`
        local -i diff
        let diff=now_size-last_size
        echo "$now_size" > $file

        #echo $last_size
        #echo $now_size
        #echo $diff
        if [ $diff -gt 3000 ];then
                send_alarm "$path log change ${diff}KB in one minutes"
        fi

        cd $cur_path
}


all_path=`cat path_list.txt`
for path in ${all_path}
do
        #echo "=====================================>${path}"
        name=`basename ${path}`
        dname=`dirname ${path}`
        pids=`ps aux | grep ${name} | grep -v grep | grep -v stop | awk '{print $2}'`
        exe_exist=0
        for pid in ${pids}
        do
                pa=`ls -l /proc/${pid} | grep cwd| awk '{print $(NF)}'`
                if [ ${pa} = ${dname} ];then
                exe_exist=1
                check_log $dname

                break
                fi
        done
        if [ ${exe_exist} = 0 ];then
                last_dir=`pwd`
                cd `dirname ${path}`;
                sh start*.sh
                cd ${last_dir}
                time=`date`
                rr="${time} ${path} restart"
                echo "$rr" >> restart_record.txt
                send_alarm "$rr"
        fi
done
