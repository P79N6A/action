#!/bin/sh
td=`date -d '7 days ago' +%s`;
echo ${td};
file=`find . -type f |grep -v $0 | xargs -I {} ls -lt --time-style='+%s' {} | awk '{if($6 < '"${td}"') print $(NF);}'`
echo "$file will be removed"
rm $file
