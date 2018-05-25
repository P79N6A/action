//1decltype可以从变量或表达式获取类型
//2decltype可以从某个变量获取类型后再进行强制类型转换，即可以把一个变量的类型转换成另一个类型（仅仅是内置
//类型可以这样强制转换，对于自定义类型，如类和函数等不可以,因为如果没有定义类型转换函数等这样一个功能，编译
//器不知道如何转化）
#include<iostream>
using namespace std;
class lei1
{
public:
	int a;
};
class lei2
{
public:
	int b;
};
void fun1(int a=1)
{cout<<"fun1 called"<<endl;
}
void fun2(int b=2,int c=3)
{cout<<"fun2 called"<<endl;}
int main()
{
	{
		int a=1;
		decltype((a)) b=a;//相当于int& b=a;定义了a的一个别名
		b++;
		cout<<a<<endl;
	}
	cout<<"----------------------------------------------"<<endl;
	{
		int a=1;double b=2;
		decltype(a+b) c=a+b;//相当于a+b的类型
		cout<<c<<endl;
	}
	cout<<"----------------------------------------------"<<endl;
	{
		double a=3;int b=3;bool c;
		c=(decltype(b))(a)==b;//可以进行这样的强制类型转换
		cout<<c<<endl;
		double *pa=&a;int *pb=&b;int *d;
		d=(decltype(pb))(pa);//也可以强制类型转换指针类型
		cout<<*d<<endl;
	}
	{
		lei1 a;lei2 b;
		lei1 c;
		//c=lei2(b);编译出错：no known conversion for argument 1 from 'lei2' to 'const lei1&'
	}
	{
		//cout<<((decltype(fun1))(fun2)==fun1)<<endl;//函数指针不能转换：error: invalid cast to function type 'void(int)'
	}
	return 0;
}