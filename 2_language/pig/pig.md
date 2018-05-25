

参考链接
========================================
<https://www.codelast.com/%E5%8E%9F%E5%88%9Bpig%E4%B8%AD%E7%9A%84%E4%B8%80%E4%BA%9B%E5%9F%BA%E7%A1%80%E6%A6%82%E5%BF%B5%E6%80%BB%E7%BB%93/>
<https://www.w3cschool.cn/apache_pig/>
<http://pig.apache.org/docs/r0.17.0/basic.html>



基本概念
========================================
理解：关系（relation）、包（bag）、元组（tuple）、字段（field）、数据（data）的关系

常见关键字
========================================
FILTER	过滤
GROUP	聚合
COGROUP	可以把两个已经聚合的目标，根据key再聚合在一起；
JOIN	根据某个字段组合两个data，两组数据会分别两两组合；



结合Shell
========================================
STREAM 
	B = STREAM A THROUGH `awk '{if($4 == "=") print $1"\t"$2"\t"$3"\t9999\t"$5; else print $0}'`;


定义常量
========================================
%declare cv	3000	定义数字常量可以直接与int类型比较${cv} >= colume;
%declare name 'xxx'	定义字符串常量
%declare cmd	`awk ....`	定义command常量
	
	
	
特别注意
========================================
用于GROUP的key如果多于一个字段（正如本文前面的例子），则GROUP之后的数据的key是一个元组（tuple），否则它就是与用于GROUP的key相同类型的东西。

flatten		在使用时最好对目标加上括号，针对?:表达式，需要加双括号才可以避免一些莫名的问题；

？：		该数学表达式的使用必须要把整个括起来，并且对？前面最好也加上括号（即使？前面是一个函数处理如IsEmpty），否则会有一些莫名的问题；
	FLATTEN(((IsEmpty(f0))?null:f0.cnt)) as v;

技巧
------------------------------
null的妙用：{
有时需要把一个在?:表达式中在一个int(0)和一个:bag{:tuple(int)}类型之间选择，但是?:表达式只支持同类型选择，且bag与int不能直接cast转换。这时可以利用null。null可以与任何类型比较，先用flatten把bag中间变量取出来，而0暂用n代替，后一步再判断Null把它赋值为0。
}

计算一个goup的key的数量{
B = group A ALL;C = foreach B generate COUNT(A.xxx) as xx1;
}

替换某种规则的字符串{
	C = foreach B1 generate type as type, rt as rt, ec as ec,timestamp as timestamp,REPLACE(ed,'.*file too long.*','download file too long') as ed ;
}

排序并取topn{
	C1 = order C by cnt desc;
	--dump C1;
	D = limit C1 10;
	--dump D;
}