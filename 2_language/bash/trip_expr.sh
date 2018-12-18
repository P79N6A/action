#!/bin/sh
echo 'if use "set -e" at the start of the script, when expr returns a value of 0, the script will exit directly
use 
(( t = a + b, 1))
(( t = a + b)) || true
let "t = a + b" 1
instead'

set -e;
#t=`expr 0 \* 1024 \* 4096 + 0 \* 4096 + 0`
((t1 = 0 * 1024 * 4096 + 0 * 4096 + 0 , 1))
((t2 = 0 * 1024 * 4096 + 0 * 4096 + 0 )) || true
let "t3 = 0 * 1024 * 4096 + 0 * 4096 + 0 " 1
echo $t1
echo $t2
echo $t3
echo 'good'

