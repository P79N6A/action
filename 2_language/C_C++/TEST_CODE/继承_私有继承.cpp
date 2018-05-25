//1that with private inheritance, a reference or pointer to a base class cannot
//be assigned a reference or pointer to a derived class without an explicit type cast
//2可以将一个派生类对象转化为一个基类，可以将一个派生类指针转换为一个基类指针
//如果派生类存在与基类相同名的函数，无论是指针调用还是对象调用，可以在函数前
//加域作用符以示区分。在函数中任何情况下，通过(base*)this和(base&)*this可以将派生类
///的指针和派生类对象转换为基类指针和基类对象的引用。

//3私有继承中，用using base::fun来重新声明基为要被转为私有函数的函数，可以使这个
//函数覆盖这个派生的私有函数，使用其变成公有函数。利用类继承的函数覆盖原理，
//可以有选择的使部分函数面对私有继承仍然保持为public函数。那个函数仍然存在
//只是被继承为了私有而已，这里是定义了一个相同的函数在公有部分代替它而已
//4基类和派生类的地址是一样的，只是指针性质不一样，就是所指对象的长度不一样
//5在派生类函数中调用基类的函数，有四种方式：一、使用派生类对象
//调用基类函数，二、使用派生类指针调用基类函数（因为基类函数属于派生类一部分）
//三、将派生类对象强制类型转换为基类对象再调用基类函数。
//四、将派生类指针强制类型转换为基类指针再调用基类函数。
//根据基类与派生类函数是否发生覆盖来判断是否加域作用符，当然即使不覆盖
//也可以加域作用符
#include<iostream>
using namespace std;
class lei
{
public:
	void fun1(){cout<<"fun1() called in lei"<<endl;}
};
class leiA:private lei
{
public:
	using lei::fun1;//用using将父类中被继承为私有函数的fun1重写为公共函数，这样可以覆盖私有函数。
	void fun2(){cout<<"fun2() called in leiA\t"<<endl;}
	void fun3(){cout<<"in fun3(),use lei::fun1():\t";lei::fun1();}//在派生类中定义有fun1时，要域作用符。属本类加域作用符调用
	void fun31(){cout<<"in fun31(),use fun1():\t";fun1();}///在派生为中没有定义fun1()时可以这样使用。使用本类调用
	void fun32(){cout<<"in fun32(),use this->fun1():\t";this->fun1();}//本类指针调用
	void fun33(){cout<<"in fun33(),use this->lei::fun1():\t";this->lei::fun1();}//加域作用 符的本类指针调用
	void fun4(){cout<<"in fun4(),use ((lei*))this->fun1():\t";((lei*)this)->fun1();}//使用基类指针调用
	void fun5(){cout<<"in fun5(),use (lei&)(*this):\t";((lei&)*this).fun1();}//因为"."的优先级高，前面整体要加括号
	//使用基类对象调用
};
int main()
{
	{
		lei *p;
		leiA b;
		//p=&b;//私有继承，不能用基类的指针指向派生为对象
		p=(lei*)&b;//可以使用强制类型转换
		p->fun1();
	} 
	cout<<"////////////////////////////////////////////////////////////////"<<endl;
	{
		leiA a;
		a.fun1();

		a.fun2();
		a.fun3();
		a.fun31();
		a.fun32();
		a.fun33();
		a.fun4();
		a.fun5();
	}
	cout<<"////////////////////////////////////////////////////////////////"<<endl;
	{
		leiA a;
		cout<<"&a:"<<&a<<endl
			<<"(lei*)&a:"<<(lei*)&a<<endl;
	}
	return 0;
}