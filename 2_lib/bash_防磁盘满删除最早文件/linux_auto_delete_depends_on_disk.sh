ad=`df | grep 'data1' | awk '{print $(NF-1)}' | cut -d '%' -f 1`
if [ $ad -gt 95 ];then
	file=`find . -type f |grep -v 'auto'| xargs -I {} ls -lt --time-style='+%s' {}| sort -k 6 | head -10 | awk '{print $NF }'`
	echo "the use rate of disk use is over $ad, file:$file will be removed"
	rm $file
fi
