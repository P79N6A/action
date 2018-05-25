g++
===========================


安装
---------------------------
yum install gcc-c++


g++ -fPIC -shared -o libdemo.so file1.cpp fil2.o libfile3.a		生成成so文件；


-Wl,--whole-archive	使静态库无条件全部链接进可执行程序；