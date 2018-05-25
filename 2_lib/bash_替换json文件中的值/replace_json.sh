#!/bin/sh
function rpl 
{
        cat $1 | sed  '/'$2'/{s@\(^[^:]*:"*\)[^",]*\(.*$\)@\1'$3'\2@g}'
}
rpl config.json roomid 314