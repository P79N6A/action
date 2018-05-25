//1在C++11标准中，类成员可以在声明时初始化，相当于使用初始化列表。
//2类中，构造函数、类型转换函数、析构函数、复制构造函数、赋值运算符是五个很典型的函数。
//其中构造函数和析构函数描述了一个类对象的建立和消亡。构造函数和类型转换函数服务于类对象
//与内置类型数据混合运算（explicit修饰很重要）。复制构造函数和赋值运算符服务于类对象
//间运算和类对象与内置类型的混合运算，它们是由一种数据类型构造另一种数据类型的有效方式。
//严格说来，复制构造函数也是构造函数，构造函数和析构函数是类对象生死机制。
//构造函数和赋值运算符是由一种类型向另一类型赋值和克隆的有效方式。函数调用者或者说
//被转化目标类型始终只能是类。
//类型转换函数，则是类向其它类型转换的有效方式。
//3只要使用new 而不是replacement new分配的内存，就要自定义复制构造函数，赋值运算符
//和析构函数。
//4如果类内的函数只是作了声明，而没有定义，会在链接时出现无法解析的外部符号的错误（如果主函数
///中用到了这个函数）
//5自己定义的非复制构造函数并不会覆盖编译器默认创建的复制构造函数，所以仍然可以使用
//默认的复制构造函数
/////////////////////////////////////////////////////////////////////////////////////////
//=>C++11标准
//在C++11新标准中，类可以在定义时进制初始化，如下下面leiA就是如此。
#include<iostream>
#include<string>
using namespace std;
class leiA
{
public:
	int a=10;//在C++11标准中可以这样写
	string str="thinking";
private:
	//leiA(){}//如果将默认的构造函数声明为private，那么默认的构造函数相当于被delete，在没有
	//定义其它构造函数时，它将是一个矛盾类，无论如何也不能定义出一个对象来。
};

int main()
{
	{
		leiA a;
		cout<<a.a<<endl
			<<a.str<<endl;
	}
	{
		//如果类中有动态分配的内存，在没有自定义复制构造函数和赋值运算符的情况下，
		//为防止误用复制构造函数和赋值运算符而导致的程序问题，可以定义如下example
		// 中简单的复制构造函数和赋值运算符并用private声明。在C++11中可以使用
		///delete关键字来disable某个函数。
		//《C++ Primer Plust 6th》P90
		//class Queue
		//{
		//private:
		//Queue(const Queue & q) : qsize(0) { } // preemptive definition
		//Queue & operator=(const Queue & q) { return *this;}
		////...
		//};
		//This has two effects. First, it overrides the default method definitions that otherwise
		//would be generated automatically. Second, because these methods are private, they can’t
		//be used by the world at large.That is, if nip and tuck are Queue objects, the compiler
		//won’t allow the following:
		//Queue snick(nip); // not allowed
		//tuck = nip; // not allowed
	}
	return 0;
}