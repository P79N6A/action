#!/bin/sh
chmod +x first.sh
./first.sh
source ./second.sh "pa1";
sh third.sh
echo '--------------'
echo $first;
echo $second;
echo $third;
