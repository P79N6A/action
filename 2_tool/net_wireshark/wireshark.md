

wireshark
=====================================
https://www.wireshark.org/docs/man-pages/wireshark-filter.html



按IP过滤
--------------------------------------
ip.dst,ip.src,ip.addr


按端口过滤
--------------------------------------
tcp.port==1234
tcp.dstport==1234
tcp.port>=10000 and tcp.port<=20000
tcp.port in {80 443 8080}	端口可以用in;也可以用and写成多个条件；



按包内容过滤
--------------------------------------
tcp contains 1c-02-86-ab-c2-1e-00-00-00	过滤包含某段二进制数据的包
ftp[1,3-5,9:] == 01:03:04:05:09:0a:0b	指定偏移处的二进制过滤
tcp contains string						过滤包含字符串string的包（string不用打引号）
tcp[20:30]	contains strings			指定位置是否包含,20表示对应协议偏移，30表示长度；（注意这里20的偏移是按tcp层的地址来偏移）
frame[-4:] == 0.1.2.3					偏移位置可以是负的(frame是按整个包数据来算偏移)
udp[8:2]==29:29			UDP头MAGIC是0x2929（按UDP层的数据开始位置算偏移）

数据包偏移
--------------------------------------
由tcpdump抓包得到的UDP数据，在wireshark中看具体数据是从0x2C(44)位置开始；抓到的包中前16个字节是MAC地址相关信息，比MAC头长度14还多出来两个字节（多出来两个名为Unused），然后IP头长度为20，UDP头长度为8，一共44字节（16+8+20=44）



TIPS
--------------------------------------
windows下抓包有时候看不到可选的本地连接，需要启动NPF，在CMD窗口中运行net start npf
