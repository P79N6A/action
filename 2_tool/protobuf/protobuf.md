
protobuf
================================
<https://developers.google.com/protocol-buffers/>

主要支持的功能有：
---------------------
+ 跨语言通信支持
+ 支持新协议发向老服务而无需修改代码；新协议数据被老服务再次序列化发送到其它新协议服务，仍然可以正常解析到新协议字段；老协议服务向新协议服务发送消息，新协议服务直接获取新字段的内容得到的是默认值，在pb2中可用has_***来判断,在pb3中整型会返回0，字符串会返回空，pb3中没有has_***函数；
+ 如果对原协议做减法；那么原协议中要被删除除的字段的名称或tag数字要用reserved字段来修饰，以避免再次添加同名字段导致异常；如果要修改协议中的字段，int,int64等整型可以保持兼容直接修改，但其它字段不可以；
+	定义基于pb的rpc services


细节功能
---------------------
	package foo.bar		定义命名空间
	option optimize_for = CODE_SIZE;	*.proto文件中加它使生成更少的代码
	option optimize_for = LITE_RUNTIME;	*.proto文件中加它使生成更运行更快的代码
	option cc_enable_arenas = true;		Arena功能，可以使用它提前为message new一个对象，避免重复分配内容；
	enum		定义枚举
	optioinal 	选填项
	required	必填项
	repeated	在pb2中添加[packed=true],可以使其更高效，在pb3中默认开启；
	oneof		相当于C中的共同体；
	maps		可以直接传map；对端可用map接收或数组接收；
	extension	可在其它文件或同文件其它位置添加扩展字段；引用或赋值时与一般字段存在区别；
	import		引入其它文件
	可以嵌套定义类；

	
命令行
---------------------
	protoc -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/addressbook.proto
	protoc --encode=MSGTYPE a.proto > binary_file	通过标准输入来编码一个文本消息。(长度必须严格匹配，输入过长或过少的长度均会解析出错)
	cat binary_file | protoc --decode=MSGTYPE a.proto > text_file		可通过标准输入向该程序输入一个二进制生成文本 
	protoc --decode_raw		不需要协议直接解析二进制；(长度必须严格匹配，输入过长或过少的长度均会解析出错)
	
特别注意的地方
---------------------
* 如果强制把一个int赋值给一个枚举值，如果不在枚举值范围内，在pb2中会引发runtime crash；
* 一个结构体内的某个required变量没有赋值，仍然可以序列化成功并发送，但是接收端会出现parse error;发送端可以调用IsInitlized()来判断所有required是否都填了；
