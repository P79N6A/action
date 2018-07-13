

tcpdump
============================
http://www.tcpdump.org/

抓包文件格式
----------------------------
http://www.tcpdump.org/pcap/pcap.html
http://www.kroosec.com/2012/10/a-look-at-pcap-file-format.html

可以抓包时过滤或保存后多层过滤包。每次过滤后都可以使用-e,-X等选项来查看。

可以指定想要抓的包个数，抓包时长，抓包文件大小（可以连续保存在多个文件）


使用过滤
----------------------------
不同的过滤条件可以写在文件中，用-F来加载。

可以把原始数据-w保存，然后用-r读取过滤，每次-r的同时可以-w保存过滤结果。

按包内字符串过滤：cat file|sed -n '/string/p' > result

按包内二进制过滤：sed -n '/\xfa\xfb\xfc\xfd/' file > result

trips
----------------------------
less,greater关键字是按去除MAC头后从IP头开始算的长度（即如对UDP来讲，比较的数应该是28+数据长度）。（个人实践的结论）

组合条件
----------------------------
非 : ! or “not” (去掉双引号)
且 : && or “and”
或 : || or “or”
如果抓包条件是and or 等混合的条件是用小括号（）括起来，并且所有条件要加单引号或双引号，如
tcpdump -i any 'udp host 127.0.0.1 and (port 10001 or port 10002))'
tcpdump -i any "(port 80 or port 443) and ((host 172.17.30.100 or host 172.17.30.101 or host 172.17.30.102) and ((host 172.17.31.200 or host 172.17.31.201 or host 172.17.31.202))"
tcpdump host ip1 and (ip2 or ip3)
tcpdump host ip1 and !ip2