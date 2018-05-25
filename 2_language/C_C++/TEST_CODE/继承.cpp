//1虚函数在派生类中被重写时，函数类型必须完全一致，包括加不加const等
//在一个类中定义两个一样的成员函数，一个加const，一个不加const。实际调用时，
//非const对象调用非const函数，const对象调用const函数。
//2如果基类的成员函数有const，若派生类中重写时不加const，则不视为重写，相当
//于定义了一个新函数。
//3基类指针或引用只能调用由基类或派生类中从该基类派生的成员，而不能调用
//派生类从另一个父类继承的成员。
//4将基类对象的指针强制类型转换为派生类指针，用该指针支调用派生类有而基类
//没有的对象，编译不出错，但会输出一个乱码。同时可以正常调用派生独有的函数。
//然后用这个转换后的指针释放内存没有问题。作用dynamic_cast可以避免这种不安全
//的转换，不安全时返回的指针是0.
//5不同域中，派生类和基类类名可以一样
#include<iostream>
#include<cstdlib>
using namespace std;
class A
{
public:
	virtual void show1(int in)const {cout<<"A called with:"<<in<<endl;}
	virtual void display(){cout<<"A function display called"<<endl;}
	void display1(){cout<<"function display1() called in class A"<<endl;}
};
class B
{
public:
	virtual void show2(){cout<<"B called"<<endl;}
};
class C:public A,public B
{
public:
	C(){c=100;}
	virtual void show1(){cout<<"C called"<<endl;}
	virtual void show1(int in){cout<<"C called with:"<<in<<endl;}
	virtual void show1(int in)const{cout<<"C const function show1 called with:"<<in<<endl;}
	void display1(){cout<<"function display1() called in class C"<<endl;}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void showC(){cout<<"showC called in class C";}
	int c;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//class A:public A//同一作用域中不能同名继承
//{};
class string:public std::string//不同域中，派生类和基类类名可以一样
{

};
int main()
{
	{
		C c;
		c.show1(0);//调用非const show1
		const C ca;
		ca.show1(0);//调用const show1
		A*a=&c;
		a->show1(1);//调用重写函数
		a->A::show1(2);///调用基类的函数。
	}
	cout<<"-------------------------------------------------------------"<<endl;
	{
		C c;
		B *b=&c;
		//b->display();//error C2039: “display”: 不是“B”的成员
	}
	cout<<"-------------------------------------------------------------"<<endl;
	{
		C c;
		c.display1();
	}
	cout<<"-------------------------------------------------------------"<<endl;
	cout<<"基类指针强制类型转换为派生类指针>>>>>>>>>>>>"<<endl;
	{
		A a;
		C *c;
		c=(C*)&a;
		c->showC();
		cout<<c->c<<endl;//编译不会出错，因为对象a根本没有c这个成员，
		//所以输出的这个c是编译器按在类C中寻找c的间接移动地址去寻找
		//的，所以它输出的一定是一个乱码

		A *aa=new A;
		C *d=(C*)aa;
		delete d;
	}
	return 0;
}