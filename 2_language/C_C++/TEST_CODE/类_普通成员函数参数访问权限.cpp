//1在类的成员函数中可以使用包括同类型局部变量，同类型参数的私有成员，如果不是同类型则不可以；于是包括复制构造函数，赋值运算符在内的函数，以及其它所有普通函数都可以访问this类成员，局部对象以及参数同同类对象的私有成员。看似破坏了类的封装性，其实不然，因为如果试图利用这一点来在一个类对象的成员函数中来改变别一个类对象的私有成员值，那么既然这两个对象是同类的，那么另一个类对象同样存在一个相同的函数来改变其它同类的私有成员，当然这个参数也可以是自己，所以它一定可以通过这个函数来改变自己的私有成员值。这样的结果是：一个对象可以通过私有成员函数来改变另一个同类对象的私有成员。经过尝试确实是这样。

#include <iostream>
using namespace std;
class A
{
private:
	int num;
public:
	A(){num=100;}
	A add(int i,A&c);
	int value(){return num;}
};
A A::add(int i,A&c)
{
	A a;a.num+=i;
	c.num=-17;
	return a;
}
int main()
{
	A a;
	A c;
	A b=a.add(10,c);
	cout<<b.value()<<endl;
	cout<<c.value()<<endl;
}



