//1所有静态变量在编译时均会被初始化为0，编译完之后被初始化为设定的值（见下面的测试）
//2在模板中通过对函数的指针进行比较，可以实现在模板中对不同的函数进行处理（函数地址相当于ID）
//3也可以向模板或普通函数传递一个int量作为ID，根据这个ID来调用不同的函数，如useFun(int flag);当
//flag==1时，调用fun1,当flag==2时，调用fun2.
//4上面2和3的思想都通过向函数传递一个ID或者说是代表或者说是信息，让接口函数useFun来判断调用
//哪个函数，而不是调用传递过去的函数指针，如果使用传递过去的函数指针得使用条件编译或者是统一
//参数类型
//5当然可以把函数打包成一个类，用对象进行传递。

#include<iostream>
using namespace std;
const int b=2;
#define RUN 100
void fun1(int a)
{
	cout<<"fun1 called with "<<a<<endl;
}
void fun2(int b,int c)
{
	cout<<"fun2 called with "<<b<<" and "<<c<<endl;
}
template<typename funT>
void useFun(funT fun)
{
	cout<<"fun address in template:"<<fun<<endl;
	if(long(fun)==long(fun1))//在g++下，fun1在使用之前要声明，所以把定义移到前面，也可以前面多一个声明，而在vc下编译
		//下可以不用在意函数的顺序
		//通过比较函数的指针可以实现在模板中调用不同的函数，即统一的模板接口调用不同的函数
	{
		cout<<"fun==fun1"<<endl;
		fun1(100);
	}
	     
	if(long(fun)==long(fun2))
	{
		cout<<"fun==fun2"<<endl;
		fun2(200,300);
	}
//#if (long)(fun)==(long)(fun2)//利用条件编译没有成功：表达式在误，编译阶段只需要特定的常量表达式
////	cout<<"B"<<endl;
////	fun(200,300);
//#elif (long)(fun)==(long)(fun1)
//	//cout<<"A"<<endl;
//	//fun(100);
//#endif
	
}
int main()
{
	{
		const int a=1;
#if !b//b的值是在编译时被初始化为0，而在编译完之后被初始化为2，所以条件编译!b为真，而b为假
		cout<<"ha"<<endl;
#endif
#if b
		int c;
		c=b+1;
		cout<<"ppy";
#endif
#ifdef RUN
		cout<<"Yes"<<endl;
		cout<<a<<'\t'<<b<<endl;
#endif
	}
	cout<<"-----------------------------------------------------------------"<<endl;
	{
		cout<<"fun1:"<<fun1<<endl;
		cout<<"fun2:"<<fun2<<endl;
		//if(fun1!=fun2)//编译出错，不能比较：error C2446: “!=”: 没有从“void (__cdecl *)(int,int)”到“void (__cdecl *)(int)”的转换
//1>        该转换要求 reinterpret_cast、C 样式转换或函数类型转换
		//if((&fun1)!=(&fun2))//编译出错，错误同上
		if(long(fun1)!=long(fun2))///强制类型转换为long类型进行比较，不能使用decltype将一个类型转换为另一个类型，因为
			///decltype的强制类型转换只能针对build-in类型，自己在decltype中测试过
			cout<<"fun1!=fun2"<<endl;
		//useFun<decltype(fun1)>(fun1);//也可以这样实例化
		useFun(fun1);
		useFun(fun2);
	}
	return 0;
}