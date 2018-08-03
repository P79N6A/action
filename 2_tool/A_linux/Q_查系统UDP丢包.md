netstat查看系统socket buffer是否不足引起丢包。
使用watch -n 1 'netstat -su'。如果SndbufErrors发生变化，说明系统层发生丢包。需要用sysctl -a或sysctl -w来查看或设置socket缓冲区。
netstat -anp 				结果第一列和第二列显示接收和发送缓冲区现在堆积的数据（man netstat可以看到解释）

tc -s qdisc show dev eth2	查看网卡是否丢包；

sar -n EDEV 1或ifconfig		查看网卡层面是否丢包。