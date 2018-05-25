netstat查看系统socket buffer是否不足引起丢包。
使用watch -n 1 'netstat -su'。如果SndbufErrors发生变化，说明系统层发生丢包。需要用sysctl -a或sysctl -w来查看或设置socket缓冲区。

sar -n EDEV 1或ifconfig查看网卡层面是否丢包。