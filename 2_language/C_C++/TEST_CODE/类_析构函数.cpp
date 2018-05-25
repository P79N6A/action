//1显式调用析构函数后，类对象并没有被销毁，对象的真正销毁是在其离开定义域，离开定义域
//后编译器会再次调用析构函数。（第一次调用析构函数后，如果类中存在动态分配的内存
//必须再次分配一次，否则系统再次调用析构函数，会使得重复删除同一内存出现崩溃）
//2临时变量的销毁时间取决于编译器，有时刚用完就销毁，如下面的测试程序在"///////////"输出
//前销毁，有的可能在离开定义域后销毁。
//3将析构函数声明为私有会导致其无法被编译器调用，而导致编译错误
#include<iostream>
using namespace std;
class leiA
{
public:
	leiA(){
		p=new double(3);
		a=10;
	}
	~leiA(){cout<<"~leiA() called with delete "<<*p<<endl;delete p;}
	int a;
	leiA& operator=(leiA&cl)
	{
		if(&cl==this)
			return *this;
		a=cl.a;
		*p=*cl.p;
		return *this;
	}
public:
	double *p;
};
/////////////////////////////////////////////////////////////////////////////
class leiB
{
public:
	int a;
private:
	~leiB(){}//将析构函数声明为私有
};
int main()
{
	{
		leiA a;
		a.~leiA();
		cout<<"a.a:"<<a.a<<endl;
		a.p=new double(100);
	}
	cout<<"-----------------------------------------------"<<endl;
	{
		leiA a;
		a=leiA();
		cout<<"//////////////////"<<endl;
	}
	cout<<"-----------------------------------------------"<<endl;
	{
		cout<<"=>将析构函数声明为私有"<<endl;
		//leiB b;
	}

	return 0;
}