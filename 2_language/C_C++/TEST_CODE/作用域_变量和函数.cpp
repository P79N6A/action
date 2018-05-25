//1在同一个scope中，变量名和函数名不能相同，如同一类中成员函数和变量，全局域中的函数和变量。
//2类作用域中的变量名会屏蔽全局域中的变量名和函数名，类作用域中的函数名会屏蔽全局域中的变量名和函数名
//3函数和变量虽然角色不同，但都可看作是变量，也就是说变量名和函数名在同一域中不能冲突，在不
//同域中遵循屏蔽规则
#include<iostream>
using namespace std;
void fun1()
{}
int fun2()
{return 2;}
int fun3=0;
int fun5()
{return 5;}
//int fun5; error C2365: “fun5”: 重定义；以前的定义是“函数”
class lei
{
public:
	lei()
	{
		//fun3=33;//编译出错： error C2659: “=”: 作为左操作数，这个fun3编译器认为使用的是成员函数
	}
	int fun3()
	{
		return 333;
	}
	int fun4()
	{
		return 444;
	}
	//int fun4;//编译出错：error C2365: “lei::fun4”: 重定义；以前的定义是“成员函数”
};
int main()
{
	{
		int fun1;
		fun1=11;
		cout<<fun1<<endl;//不会输出函数地址，而是输出变量值
	}
	{
		int fun2;
		fun2=22;
		cout<<fun2<<endl;
	}
	return 0;
}