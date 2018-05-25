//1在类的复制构造函数和赋值运算符函数中分别定义以左值引用和右值引用为形参的函数，这样在实际运算中可以根据不同的
//需要调用函数，可以大大增加运行效率。
//2自己在类的构造函数中使用全局变量记录了对象的地址（当然也可以知道隐式临时对象的地址），在记录了隐式临时对象的
//地址后，发现与用这个右值构造出来的对象地址是一样的，也就证明，编译器在右值构造函数中聪明地把右值直接当成
//新变量的做法。所以，以右值为形参的构造函数不用写函数实现过程，定义这个函数，只是说明存在这样一右值构造，
//编译器知道如何构造，故不用定义如何构造，按书上的介绍，GCC会这样优化，而VC++却不会，按时要按步就搬地执行。

//
#include<iostream>
#include<cstring>//在C++11标准中，strcpy只在cstring头文件中，而不在string中
using namespace std;
long dizhi;
class lei
{
public:
	lei(const char*a="default string")
	{
		p=new char[strlen(a)+1];
		strcpy(p,a);
		dizhi=long(this);
	}
	lei(lei&cl)
	{
		cout<<"function \"lei(lei&cl)\" called"<<endl;
		p=new char[strlen(cl.p)+1];strcpy(p,cl.p);
	}
	lei(lei&&cl)//自己家义在这个函数在下面的测试中没有不行，但注释其中的内容不影响结果，也就是说函数内容没有运行，
		//尚不知为什么，可能编译器比较智能，以一个右值来构造一个类，编译器直接将这个右值作为这个新对象，只是变
		//一下名字而已，经过测试这个推断可能成立
	{
	//	//cout<<"function \"lei(lei&&cl)\" called"<<endl;
	//	//p=cl.p;cl.p=0;
	}
	~lei(){delete[]p;}
	lei& operator=(lei&cl)
	{
		cout<<"function \"lei operator=(lei&cl)\" called"<<endl;;
		if(this==&cl)return *this;
		delete []p;;
		p=new char[strlen(cl.p)+1];
		strcpy(p,cl.p);
		return *this;
	}
	lei& operator=(lei&&cl)
	{
		cout<<"function \"lei operator=(lei&&cl)\" called"<<endl;
		p=cl.p;cl.p=0;
		return *this;
	}
	lei* address()
	{
		return this;
	}
public:
	char *p;
};
int main()
{
	{
		lei cl("happy from hard");;
		lei a(cl);
		//cout<<cl.this<<endl;不能直接调用this，可以利用函数调用或者使用地址运算符
		a=cl;//调用以左值引用为形参的赋值函数
		a=lei("successfully");//调用以右值引用为形参的赋值函数
	}
	cout<<"--------------------------------------------"<<endl;
	{
		lei b(lei("all over my life"));//编译器会直接把右值当成新变量
		//lei b;
		cout<<b.p<<endl;
		//lei c(b=lei("the more harder, the more interesting"));
		cout<<long(&b)<<endl;
		cout<<b.address()<<endl;
		cout<<long(b.address())<<endl;
		cout<<dizhi<<endl;
	}
	return 0;
}