
参考文献
==========================
https://www.gnu.org/software/gdb/

帮助
==========================
	help
	help command

获取信息
--------------------------
	bt		获取当前堆栈
	ptype	打印变量类型
	p		打印变量
	p *array@len	打印数组
	dump	把指定内存变量等信息打印到文件进行处理

控制运行
--------------------------
	b,break	设置断点
	watch	设置观察点
	c,continue
	finish
	until

保存断点
--------------------------
	save b file	把所有断点保存到file中；
	source file 运行外部命令文件，可以是断点文件，也可以是自定义命令文件；
	
反汇编
--------------------------
	disas


自定义命令
--------------------------
	set $name=value		设置变量及其值
	for
	while
	if
