机器ssh登陆操作很卡
----
用lsof查看已经打开的文件是不是太多（打开很多无效的fileid会导致CPU不高，网络流量很低，内存也足，但是很卡的情况）
实时查看不同进程的网络流量
----
开源工具nethogs
----
网络诊断
====
	mtr ,ping、netstat、traceroute
网络抓包工具
====
	sniffer、ethereal、tcpdump
网络性能测试工具
====
	iperf
	
LINUX平台上的常用监控工具
====
    top、vmstat、sar、strace、ltrace

程序性能监控工具
====
   Gprof、Rational  Quantify 、perf
   
文件操作
==================================================
readlink -f file	查找多次被软连接的真实文件路径；