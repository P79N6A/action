//1双目运算符重载为友元函数，在使用时，第一个参数和第二个参数分别位于双目运算符
//的两边，而运算符的调用者可以是第一个参数，也可以是第二个参数。
//双目运算符重载为成员函数，在使用时，在双目运算符左边的是调用者，右边的是参数。
//2无论是单目运算符，还是双目运算符，运算符重载函数返回类型则可以是任何类型
//（如调用者的引用、bool类型、内置类型等）。
//3绝大部分运算符可以重载为普通函数（全局静态），而运算符[]，()，=，->只能重载
//为非静态成员。双目运算符重载为普通函数时，至少要有一个类类型形参。
//4基类的重载运算符函数可以被派生类继承（它本质上和成员函数没有区别）

#include<iostream>
#include<string>
using namespace std;
class lei
{
public:
	lei(){str="lei";num=0;}
	string str;
	int num;
};
class leiC;
class leiB;
class leiA:public lei
{
public:
	leiA(string s="leiA",int n=1){str=s;num=n;}
	//leiC operator+(leiB&b);
};
class leiB:public lei
{
public:
	leiB(string s="leiB",int n=2){str=s;num=n;}
	friend leiC operator+(leiA&a,leiB&b);
};
class leiC:public lei
{
public:
	leiC(string s="leiC",int n=3){str=s;num=n;}
};
//leiC leiA::operator+(leiB&b)
//{
//	return leiC(str+b.str);
//}
leiC operator+(leiA&a,leiB&b)
{
	return leiC(b.str,a.num+b.num);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//int operator+(int a,int b)//必须至少有一个类类型的形参
//{return a+b;}
//lei operator+(lei*a,lei*b)//指针类型不能被重载
//{
//	return lei(a->str+b->str,a->num+b->num);
//}
int main()
{
	leiA a;leiB b;

	cout<<(a+b).str<<endl;
	cout<<(a+b).num<<endl;


	return 0;
}