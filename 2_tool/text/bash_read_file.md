	
从文件一行一条读取字符串
---------------------
IFS=' ';
while read -r line;do
	for x in $line;do
	echo $x;
	done
done<file

把文件映射成一个数组
---------------------
mapfile
readarray

