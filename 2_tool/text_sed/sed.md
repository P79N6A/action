
sed$
======================
<https://www.gnu.org/software/sed/manual/sed.html>
<http://www.grymoire.com/Unix/Sed.html#uh-49>

特别注意
---------------------
sed的s命令后面可以使用任意字符作间隔符，通常依实际情况选择目标文本中不存在的字符。如s@aaa@bbb@g

\1,\n代表前面匹配字符串中用小括号括起来的第1，n个字符串。如s/^\(.*\)$/\1aaa/g，在所有行尾加aaa
&代表前面整个匹配字符串。如s/aaa/&bbb/g生成aaabbb。如echo 'Hello World!' | sed $'s/World/\e[1m&\e[0m/' 生成World是绿色；
当文件是一个空文件时，sed -i '1istring' 将不起作用；

需要转义的字符	^$.*[]是默认字符,其它特殊符需要转义	
---------------------
()
{}
/


sed
---------------------
	sed "/abc/,/def/{command}"	自我总结的模式（未完全确定）
	sed "5a good sun" file	//第5行之后添加一行
	sed "5i insert content" file	//第5行前面插入一行
	sed "5,7d" file		//删除第5，7行
	sed -n "5,7p" file	//打印5，行
	sed "s/aaa/bbb/g"	//把aaa换成bbb
	sed "/aaa/d"		//匹配含有aaa的行，并把它删掉
	sed "/bbb/{s/aaa/ccc/g;q}"	//匹配所有含有bbb的行，并把其中的aaa换成ccc；其中{}中可以执行多个命令，用;隔开，q是退出；

	cat file | sed -e "2d" -e "s/old/new/g"	//连续编辑先删除第2行，再把其中的old换成new
	sed -i "1i insert something"	//用-i 先项直接编辑文件
	sed ':a;N;!ba;s/\n/ /g' file	//替换文本中所有换行符\n为空格
	sed -e '/{{content}}/rt.html' -e '/{{content}}/d' file	//把文件file中含"{{content}}的部分替换成t.html文件所包含的内容
	sed 'n,/content/s/content/g'	//只处理前n+1个匹配的行

简要调试方法
---------------------
seq	n	该命令可以生成1-n的数字；后面接sed进行调试



^ 匹配行开始，如：/^sed/匹配所有以sed开头的行。 
$ 匹配行结束，如：/sed$/匹配所有以sed结尾的行。 
. 匹配一个非换行符的任意字符，如：/s.d/匹配s后接一个任意字符，最后是d。
* 匹配0个或多个字符，如：/*sed/匹配所有模板是一个或多个空格后紧跟sed的行。 
[] 匹配一个指定范围内的字符，如/[ss]ed/匹配sed和Sed。 
[^] 匹配一个不在指定范围内的字符，如：/[^A-RT-Z]ed/匹配不包含A-R和T-Z的一个字母开头，紧跟ed的行。 
\(..\) 匹配子串，保存匹配的字符，如s/\(love\)able/\1rs，loveable被替换成lovers。 
& 保存搜索字符用来替换其他字符，如s/love/**&**/，love这成**love**。 
\< 匹配单词的开始，如:/\ 匹配单词的结束，如/love\>/匹配包含以love结尾的单词的行。 
x\{m\} 重复字符x，m次，如：/0\{5\}/匹配包含5个0的行。 
x\{m,\} 重复字符x，至少m次，如：/0\{5,\}/匹配至少有5个0的行。 
x\{m,n\} 重复字符x，至少m次，不多于n次，如：/0\{5,10\}/匹配5~10个0的行。 

来自: http://man.linuxde.net/sed


