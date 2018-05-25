#include<iostream>
#include"namespace1.h"
namespace test2//将它写在头文件中包含更加方便如test3,,这里只是为了测试
{
	extern double pi;//这里只是一个声明:pi在其它文件中定义
}
namespace test3//在该cpp中要具体定义
{
	double e=2.71828;
}
using namespace std;
extern int quanju;
//int quanju1=333;//编译出错：error LNK2005: "int quanju1" (?quanju1@@3HA) 已经在 namespace.obj 中定义

void fun1()
{
	cout<<"fun1 called in fun1 of namespace2.cpp"<<endl;
	cout<<"quanju:"<<quanju<<endl;
	cout<<"test2::pi:"<<test2::pi<<"\t";
}