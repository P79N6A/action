////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//C++11标准：
//1在虚函数参数后面用于修饰函数是否要被重写的overide和final不是关键字，只是一个编译标识符，这两个word还是可以被用作变量名等
//2在类的构造和类型转换中使用explicit来说明是否虚式使用；用static,friend声明函数使用属于域的概念，用const声明是否为常函数。在使用虚函数时注意使用virtual,overirde和final。
//3养成习惯，对于构造函数和类型转换函数最好都加上explict再依据情况放开显式使用条件。
//对于使用默认复制构造函数，转移构造函数，赋值运算符和转移赋值运算符可以满足基本的需不，除非需要特别的定制。这时，使用default和delete就很必要。
#include<iostream>
using namespace std;
class lei0
{
public:
	virtual void fun2()=0;
	virtual void fun3()=0;
};
class lei:public lei0
{
public:
	explicit lei(int);//前面用explict声明构造函数或类型转换函数
	static void fun1();//用static将一个成员函数声明为静态的
	friend void fun5();//用friedn卖力一个友元函数。
	void fun4()const;//用const声明一个常类型函数
	lei()=default;//用default来说明使用默认的构造函数
	lei(const lei&)=delete;//用delete说明不使用默认的复制构造函数;也可用它来屏蔽基类的某个函数
	lei&operator=(lei&&)=default;//使用默认的转移赋值运算符
	virtual void fun2() override{}//用virtual声明一个虚函数,且编译器会根据这个override判断与基类中被重写的虚函数类型是否匹配。
	virtual void fun3() final;//用final阻止fun3()被重写。The specifier final addresses a different issue.You may find that you want to prohibit derived classes from overriding a particular virtual method.To do so, place final after the parameter list.
	//The specifiers override and final do not quite have the status of keywords. Instead,
//they are labeled “identifiers with special meaning.”This means that the compiler uses the
//context in which they appear to decide if they have a special meaning. In other contexts,
//they can be used as ordinary identifiers (for example, as variable names or enumerations).

};
int main()
{
	int override=10;
	return 0;
}