//1First, if you redefine an inherited
//method, you need to make sure you match the original prototype exactly
//如果输出仅仅是输出类型不一致,编译会出错。如果输出类型分别是基类和派生类型的引用或指针
//视为正确的重写。见《C++ Primer Plus》6th,P744
//2Second, if the base class declaration is overloaded, you need to redefine all the baseclass
//versions in the derived class。如果不需要都写，则基类的函数要使用显式调用即使用base::来调用。
//因为基类没有重写的会被覆盖，可以见下面例子。
//3 如果派生类中的函数名与基类函数名一样，但是参数类型不一样，不是基类虚函数的重写，
//那么用基类的指针调用派生类对象的函数，则会编译错误，加域作用符也不可以，派生类的
//函数名会被覆盖，只能调用基类的该名函数。调用派生类的这个函数只能使用派生类对象
//或派生类的指针，相当于完全的另一个函数，与新函数遵循同样的规则：派生类中新定义
//的函数不能由基类指针调用。
//4总结：当派生类中存在与基类中同样的函数名时，如果它是基类函数正确的重写。那么它
//会覆盖基类中所有同名函数。如果它不是基类函数的正确重写，它相当于一个新函数，
//与新函数的使用规则一致。
#include<iostream>
#include<string>
using namespace std;
class Hovel;
class Dwelling
{
public:
// three overloaded showperks()
	virtual void showperks(int a) const{cout<<"showperks(int a) const called in Dwelling"<<endl;}
	virtual void showperks(double x) const{cout<<"showperks(double x) const called in Dwelling"<<endl;}
	virtual void showperks() const{cout<<"showperks()const called in Dwelling"<<endl;}
	virtual void showperks(string a)const{cout<<a<<endl;}
	virtual int fun1(){return 1;}
	virtual Dwelling& fun2(){cout<<"Dwelling& fun2(Dwelling&d) called"<<endl;return *(new Dwelling());}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void fun3(int a,int b){cout<<"fun3() called in Dweilling with:"<<a<<"and"<<b<<endl;}
};
class Hovel : public Dwelling
{
public:
// three redefined showperks()
	virtual void showperks(int a) const{cout<<"showperks()const called in Hovel"<<endl;};
//virtual void showperks(double x) const;
//virtual void showperks() const;
	//virtual double fun1(){return 3.14;}//error C2555: “Hovel::fun1”: 重写虚函数返回类型有差异，
	//且不是来自“Dwelling::fun1”的协变
	
	virtual Hovel& fun2(){cout<<"Hovel& fun2(Hovel&h) called"<<endl;return *(new Hovel());}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void fun3(int a){cout<<"fun3() called in Hovel with:"<<a<<endl;}
	virtual void fun4(){cout<<"fun4() called in Hovel"<<endl;}
};
int main()
{
	{
		Hovel h;
		double pi=3.14;
		h.showperks(pi);//不会调用基类的符合形参为double的函数，而是将pi转化为int类型，
		//仍然使用派生类的函数
		h.Dwelling::showperks(pi);//显式调用基类的
		//h.showperks(string("successfull"));//error C2664: “Hovel::showperks”: 不能将参数 1 从“std::basic_string<_Elem,_Traits,_Ax>”转换为“int”
		
		h.Dwelling::showperks(string("successfull"));
	}
	cout<<"--------------------------------------------------------------"<<endl;
	{
		int a;double b;
		Dwelling *d,x;
		Hovel h;
		d=&h;
		cout<<(a=d->fun1())<<endl;
		cout<<(b=d->fun1())<<endl;
		
		delete &(d->fun2());
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"--------------------------------------------------------------"<<endl;
	{
		Dwelling *d,x;
		Hovel h;
		d=&h;
		d->fun3(1,2);
		//d->fun3(3);//error C2660: “Dwelling::fun3”: 函数不接受 1 个参数
		//d->Hovel::fun3(3);编译出错
		h.fun3(3);//fun3()在派生类中相当于一个新函数。

		//d->fun4();//error C2039: “fun4”: 不是“Dwelling”的成员
	}
	return 0;
}