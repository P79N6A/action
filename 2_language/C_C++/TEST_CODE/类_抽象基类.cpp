//1抽象基类在定义后，可以对其中的纯虚函数定义函数体，编译正常，但是定义的函数体没有任何作用，
//因为纯虚基类不能定义对象，纯虚函数也只有在派生类中被重写后，才能使用派生类定义对象，若派生类
//没有重写这个虚函数，派生类也将视为抽象类，同样不能定义对象。
//2定义抽象基类的指针可以指向派生类的对象。
#include<iostream>
using namespace std;
class ABC
{
public:
	virtual void show1()=0
	{cout<<"show1() called in ABC"<<endl;}
};
class leiA:public ABC
{
public:
	virtual void show1(){cout<<"show1() called in leiA"<<endl;}
};
int main()
{
	{
		ABC *pb=0;
		leiA a;
		a.show1();
		pb=&a;
		pb->show1();
	}
	return 0;
}