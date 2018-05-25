//1以auto作为函数返回值一定要在函数后面加trailing return type
//2可以使用typedef来代替一个较长的类型，或者统一代替代入的不确定类型（主要用于多
//次用到某个未知或不确定类型的情况）
//要使用C++11编译
#include<iostream>
using namespace std;
template<typename T1,typename T2>
auto add(T1 a,T2 b)->decltype(a+b)//其中->decltype(a+b)被称作trailing return type，必须要有，否则编译出错
{
	typedef decltype(a+b) outType;
	outType c;
	c=a+b;
	return c;
}
int main()
{
	cout<<add(1,'A')<<endl;
	return 0;
}
