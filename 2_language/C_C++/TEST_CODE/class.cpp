//1每个类维护着一片数据成员存储区和成员函数存储区，一个类可以有多个对象也就会有多个
//数据区，但是使用的函数都是一样的，每个类只维护一片函数区。使用sizeof得到的只是数据
//存储的长度
//2一个类对象在定义时使用赋值运算符不会调用默认重载的赋值运算符，如果带构造函数参数
//初始化，调用的就是构造函数，如果用一个已知的对象，则调用默认的复制构造函数，对于
//一个已经定义的对象作为左值使用＝，则会使用默认的或者是自己定义的赋值运算符
//3初始化列表{}能包含的值有：类构造函数的实参，数组或者向量等的成员数据，数组的成员
//变量数据，变通变量的值。所以对于一个类对象不能用{对象}的方式构造，而使用
//{构造函数参数}，所以单个对象的初始化只有（）＝和{}三种方式，其中{}中只能是构造参数
//4对于类数组可以使用{{构造函数参数}，{构造函数参数}，{构造函数参数}}的方式
//或{lei(构造参数),lei(构造参数),lei(构造参数)……}或{ob1,ob2,ob3……}ob1-ob3等是已经定义好的对象
//或{ob1,{构造参数},lei(构造参数)……}
#include<iostream>
using namespace std;
class leiA
{
public:
	int a;
	void fun(){a++;}
};
class leiB
{
public:
	int b;
};
class leiC
{
public:
	static const int c=100;//在声明类时或者说在不具体定义对象时，静态成员和函数就已经分配
	//存储空间，而普通变量却是在定义具体对象时分配存储空间
};
class leiD
{
};
leiA A;
int main()
{
	leiA a;leiB b;leiC c;leiD d;
	cout<<"sizeof a:"<<sizeof a<<endl;//a和b的长度是一样的，只计算数据的长度
	cout<<"sizeof b:"<<sizeof b<<endl;
	cout<<"sizeof c:"<<sizeof c<<endl;//相当于一个空类leiD，静态量和函数一样声明类型时就创建，且
	//与普通类型不存储在一起，它们有独立的存储位置，而且用sizeof计算，不计长度
	cout<<"sizeof d:"<<sizeof d<<endl;//空类的sizeof结果为1
	a.fun();
	cout<<"a.a:"<<a.a<<endl;//a作为一个临时变量，它的数据成员a.a不会被静态初始化为0
	cout<<"A.a:"<<A.a<<endl;//a是一个全局变量，它的数据成员A.a会被静态初始化为0
	{
		leiA c=a;//这里不会调用默认重载的赋值运算符，而是相当于leiA c(a)相当于调用默认的
		//复制构造函数
		leiA d=leiA();//这里直接调用默认构造函数，相当于leiA d();或leiA d;
		leiA e;e=leiA();//这里调用默认构造函数生成一个临时对象，再赋值给e
		leiA f(e);//调用复制构造函数
		//leiA g{e};//用C++11标准编译出错：error: cannot convert 'leiA' to 'int' in initialization
		//leiA g{}或leiA g={};//其中{}中包含的是构造函数的参数，而不能是类
		leiA h[3]={a,d};
	}
	return 0;
}