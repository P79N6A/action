
makefile
===========================
<http://www.gnu.org/software/make/manual/make.html>

可以用来构建一个工程，它可以是非C++，也可以纯shell批处理事务等；只不过主要用来编译，如果shell脚本的内容搬到makefile中则有很多符号冲突处理的情况；makefile主要还是适用于构建一个强依赖动态构建的工程。

字符串处理
---------------------------
文件抓取
---------------------------
文件全路径串处理
---------------------------
### 过滤掉其中一些文件
### 增加一些文件
### 改变文件后缀名
### 获取路径名，文件名，除后缀前半部分的名字

写C++工程的几种makefile写法
===========================
直接写法	目标和信赖对象严格手动编写，适用场景小工程，可以使得信赖关系清晰明快，没有附加文件等。
暴力写法	直接抓取所有相关文件，取路径作包含路径，直接把它作为目标项的依赖文件集。
理想写法	针对每个cpp，通过g++ -MM来生成相应的依赖文件，并把它写到文件里面；每次构建时，严格include这些文件。
务实写法	针对不同的目录和文件，可以混合采取直接，暴力及理想写法。


常见注意
===========================
makefile中每一行前面不能有空格，如果要执行一个命令，必须是一个tab，否则会报“遗漏分隔符”的错误=》可以在vim中搜索tab可以直接看到哪里是tab哪里不是tab；

换行符'\'后面不能有包括tab在内的任务字符，否则会导致当前行的下一行不被包含，即使换行符失去作用。

a.out:main.cpp main.h
	g++ -o a.out $@ $^
如果按上面这样写会存在问题，当main.cpp编译出错时，main.h会被展开并生成gch文件或生成一个precompiled a.out,导致再次运行make不会再次抛出错误即提示均已经updated。故如果使用make来构建工程，最好不要把.h当成编译目标。如可以写成如下这样：
a.out:main.cpp main.h
	g++ -o a.out $@ $(filter %.cpp %.c %.cc,$^)
或
a.out:main.cpp main.h
	g++ -o a.out $@ $(filter-out %.h,$^)

