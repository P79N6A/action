#include<iostream>
int fun1(int);
int fun2(int,int(*pfun)(int));
int fun3(int,int(&pfun)(int));
using namespace std;
int main()
{
	fun1(1);
	fun2(2,fun1);
	fun3(3,fun1);
	return 0;
}
int fun1(int in)
{
	cout<<"fun1 called with parameter:"<<in<<endl;
	return 1;
}
int fun2(int in,int(*pfun)(int))
{
	cout<<"fun2 called with parameter:"<<in<<" "<<pfun<<endl;
	cout<<"called in fun2:"<<pfun(in)<<endl;//使用指针和不使用指针调用是一样的，函数名就是指针值
	//cout<<"called in fun2:"<<(*pfun)(in)<<endl;//C++编译器视*pfun和pfun是一回事
	//参见《C++ Primer Plus》P364
	return 2;
}
int fun3(int in,int(&pfun)(int))
{
	cout<<"fun3 called with parameter:"<<in<<" "<<pfun<<endl;
	cout<<"called in fun3:"<<pfun(in)<<endl;
	return 3;
}