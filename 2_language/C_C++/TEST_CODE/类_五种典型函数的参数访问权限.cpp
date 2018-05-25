//1在构造函数和类型转换函数前面加explicit关键字是一个好习惯，根据需要放开隐式转换
//2只有同类复制构造函数和赋值运算符中的参数可以直接访问私有成员，复制类型不是本类的
//构造函数和赋值运算符参数不是本类时不可以直接访问私有成员。类型转换函数不可以
//直接访问被转换对象的私有成员。
//3只有构造函数和类型转换函数才可以用explicit修饰，explict只是针对类型转换而言
//赋值运算符析构函数等不可以。
#include<iostream>
using namespace std;
class leiB;
class leiA
{
public:
	explicit leiA(char c){ch=c;}
	explicit leiA(const leiA&ob);
	explicit leiA(const leiB&ob);
    operator leiB();
	//explicit operator leiB();//在C++11前，对类型转换不能使用explicit
    leiA&operator=(const leiA&ob);
	leiA&operator=(const leiB&ob);
	//explicit leiA&operator=(const leiB&ob);//赋值运算符不能使用explicit关键字
	//explicit ~leiA(){}//析构函数不能使用explicit修饰
	void show(){cout<<ch<<endl;}
private:
	char ch;
};
class leiB
{
public:
	explicit leiB(int in){num=in;}
	int get()const{return num;}
	void show(){cout<<num<<endl;}
private:
	int num;
};
leiA::leiA(const leiA&ob)
{
	cout<<"leiA::leiA(const leiA&ob)"<<endl;
	ch=ob.ch;
}
leiA::leiA(const leiB&ob)
{
	cout<<"leiA::leiA(const leiB&ob)"<<endl;
//ch=ob.num;//不能直接访问leiB的私有成员
	ch=(ob.get());
}
leiA::operator leiB()
{
//leiB b;b.num=ch;//不能直接访问leiB的私有成员
return leiB(ch);}
leiA&leiA::operator =(const leiA &ob)
{
	cout<<"leiA&leiA::operator =(const leiA &ob)"<<endl;
	ch=ob.ch;return *this;}
leiA&leiA::operator=(const leiB&ob)
{
	cout<<"leiA&leiA::operator=(const leiB&ob) called"<<endl;
	//ch=ob.num;//不能直接访问leiB的私有成员
	ch=ob.get();
	return *this;
}
int main()
{
	{
		leiA a('a');//显式调用
		//a='b';//不存在这样的隐式调用
		a.show();
		cout<<"------------------------------------------------------------------"<<endl;
		leiB b(67);b.show();
		a=b;a.show();
		leiB(a).show();//对a再进行类型转换。
		cout<<"------------------------------------------------------------------"<<endl;

	}
	return 0;
}