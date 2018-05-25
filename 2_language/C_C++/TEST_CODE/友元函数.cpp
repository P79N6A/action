//1不能使用成品运算符调用友元函数。友元函数是单独调用，声明在该类是指可以以该类为形参
//并用可以使用其私有成员，并用完成一些特殊的数据处理。
//2友元函数不用在类定义前作提前引用声明。
//3友元函数可以访问类的私有成员
#include<iostream>
using namespace std;
class lei
{
public:
	lei(){num=100;}
	friend void fun1();
	friend void fun2(lei&a);
	friend void fun3();
private:
	int num;
	
};
void fun1()
{cout<<"fun1 called"<<endl;}
class lei;
void fun2(lei&a)
{cout<<"fun2 called with:"<<a.num<<endl;}
void fun3(){}
int main()
{
	lei a;
	//a.fun1();//不能使用成员运算符调用友元函数
	fun2(a);
	return 0;
}