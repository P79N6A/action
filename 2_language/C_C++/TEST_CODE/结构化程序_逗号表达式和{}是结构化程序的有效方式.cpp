//如果一个模块向函数一样需要返回一个值，那么就用逗号表达式
//如果一个模块仅仅完成一些功能而不返回值，那么就用{}块
//如果一个模块中的代码要不断复用，而且只在某一个地方用，就用循环
//如果一个模块中代码要不断复用，而且要在很多地方用，就用函数
//{}和逗号表达式相当于对其中间使用的而不是本块定义的变量进行处理，所有在本块内使用
//的外部变量都相当于参数一样被传递过来处理（而且是引用传递），该块在其父块内只使用
//一次，除了for,while等循环体
#include<iostream>
#include<string>
#include<ctime>
using namespace std;
void fun1(string);
int main()
{
	{
		int a(1),b(2),c(3);
		{//{}是结构化程序的有效方法，它是一个非复用的函数，可以增强层次性和结构性
			int c;//可以定义临时变量，方便命名和操作，该模块只处理一些重要的数据
			c=a;//a,b相当于与外界的通信数据；
			a=b;
			b=c;
			cout<<"a与b互换："<<a<<" "<<b<<endl;
		}
	}
	cout<<"------------------------------------------------------"<<endl;
	{
		//for语句是一个完整的结构块，所以在for语句的初始化部分，定义的变量存在周期只限于
		//for内
	}
	cout<<"------------------------------------------------------"<<endl;
	{
		fun1(("zhang",string("ha")+string("ha"),string("successful").append("happy")));
		//逗号表达式，可以在写一个表达式的地方，完成多个表达式的功能，提高了程序的
		//聚合性和模块分明
	}
	cout<<"------------------------------------------------------"<<endl
		<<"测试{}对运算速度的影响"<<endl;
	{
		const int limit=1e9;
		clock_t s;
		s=clock();
		for(int i=0;i<limit;i++)
		{
			int a=2,b=3,c=2;
			{
				int d=3,e=7;
				a=d;b=e;
			}
		}
		cout<<"有{}耗时："<<double(clock()-s)/CLOCKS_PER_SEC<<endl;
		s=clock();
		for(int i=0;i<limit;i++)
		{
			int a=2,b=3,c=2;
			int d=3,e=7;
			a=d;b=e;
		}
		cout<<"无{}耗时："<<double(clock()-s)/CLOCKS_PER_SEC<<endl;
		cout<<"运行1e9次，差别非常小"<<endl;

	}
	return 0;
}
void fun1(string s)
{
	cout<<s<<endl;
}