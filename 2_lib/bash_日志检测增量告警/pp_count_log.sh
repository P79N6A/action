#!/bin/sh
LOGFILE='*error*'
RFILE='p_last_record.txt'
BFILE='p_crontab.bak'
SFILE='p_file_size_used_by_crontab'
ZPATH=`pwd`

echo '==================================>run crontab at '`date`
declare -A rcd
declare -A DESC
declare -A key

function send_alarm()
{
		source ~/.bashrc 	
		local ip=`ifconfig|grep cast | awk '{print $2}'`
		time=`date`	
		local data=`echo "TIME{$time}IP{$ip}ZPATH{$ZPATH}error{$1}" | sed 's/@/|/g'`
		curl -X POST -d "content:$data"	
		echo 'data has been posted:	'$data
}
function check_log()
{
        if [ ! -f "$SFILE" ];then
                echo "0" > $SFILE;
        fi
        #exit
        local last_size=`cat $SFILE`
        local all_size=(`du --max-depth=0 $LOGFILE | awk '{print $1}'`)
	local now_size=0;
	for v in ${all_size[@]}
	do
		((now_size=$now_size+$v))	
	done
        local -i diff
        ((diff=$now_size-$last_size))
        echo "$now_size" > $SFILE

        #echo $last_size
        #echo $now_size
        echo "error_file_diff=$diff"
        if [ $diff -gt 1000 ];then
                local d="log change ${diff}KB in one minutes"
		send_alarm "$d"
        fi

}
function read_last_record
{
	local i=0;
	DIFS=$IFS;
	IFS='@'
	if [ -f $RFILE ];then
		while read line
		do
			t=($line)
			k=`echo "${t[1]}" | md5sum| awk '{print $1}'`
			echo $k'=>'${t[0]}
			rcd["$k"]=${t[0]}
#			echo ${rcd["$k"]}
			DESC["$k"]=$line
			key[$i]=$k;
			((i++))
		done < $RFILE
	fi
	IFS=$DIFS;
}

#clear files that this program created
rm -rf p_tmp_* t;

if [ $# -eq 1 ] && [ $1 == 'install_crontab' ];then
	echo '#####################'> $BFILE  #avoid empty file, that sed can't i
	crontab -l >> $BFILE
	
	echo '--------------------------------check whether crontab has been installed'
	t=`cat $BFILE | grep $ZPATH | wc -l`
	if [ $t -eq 0 ];then
		echo '===================='
		sed -i "1i#add by focuszhang" $BFILE 
		sed -i "1a*/3 * * * * cd $ZPATH;sh pp_count_log.sh >> p_crontab.log" $BFILE
		sed -i "1a0 0 * * * cd $ZPATH;rm -rf p_crontab.log"  $BFILE
		cat $BFILE | crontab
		echo 'install crontab'
	fi
	exit 0;
fi


#set -e ;
echo '--------------------------------check log size first'
check_log;
echo '--------------------------------check other crontab'
n=`ps aux | grep $0 | wc -l`
if [ $n -gt 100 ];then
	echo "ERROR, too many crontab task $n"
	exit 0;
fi

exit 0;

echo '--------------------------------get new update file'
td=`date -d '3 minutes ago' +%s`;
now=`date +%s`
file=`find . -name "$LOGFILE" |grep -v $0 | xargs -I {} ls -lt --time-style='+%s' {} | awk '{if($6 > '"${td}"') print $(NF);}'`
echo "updated files=$file"
#cat $file | grep ERROR 
if [ -z ${file[0]} ];then
	exit 0;
fi

echo '--------------------------------read through log'
cat $file | grep ERROR |   sh pp_getline.sh $td $now | sed 's@^[^a-z]*\]\(.*ERROR[^0-9]*\).*$@\1@g' | sort | uniq -c | sort -n -r -k 1 | head -10 | awk '{printf("%s@%s@{%s}\n",$1,$2,$0);}' > $RFILE

echo '--------------------------------read cur_record file' 
read_last_record

echo '--------------------------------calculate diff'
echo "klen=${#key[*]}"
echo "${key[0]}"
echo "${key[1]}"
i=0;
for k in "${key[@]}"
do
#	echo $k'=>=>'${DESC[$k]}
#	echo 'key='$k
#	echo ${rcd["$k"]}
	d=${rcd[$k]}
	if [ -z $d ];then
		d=0
	fi
	echo $k'=>'${DESC[$k]}
	if [ $d -ge 1000 ];then
		send_alarm "${DESC[$k]}"
	fi

	#only send 3 line per time
	((i++))
	if [ $i -gt 3 ];then
		break;
	fi
done 

