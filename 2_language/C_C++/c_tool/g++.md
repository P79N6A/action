g++
===========================


安装
---------------------------
yum install gcc-c++


g++ -fPIC -shared -o libdemo.so file1.cpp fil2.o libfile3.a		生成成so文件；


-Wl,--whole-archive	使静态库无条件全部链接进可执行程序；


注意
---------------------------
如果把.h写入编译项，gcc为了加快编译速度会针对头文件生成头文件的预编译文件*.h.gch,如这样写g++ -o a.out main.cpp main.h会生成main.h.gch（当编译main.cpp出错时），由此还会引发make第二次编译时不会重复报出上次make出现的错误，并生成一个错误的可执行同名文件。当然如果不用make来构建工程则可以这样写，如果使用了make，则需要不把.h当成编译目标。