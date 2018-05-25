//1typeid的参数可以是变量或者类型
//2无法在类中定义type_info的变量，You cannot instantiate objects of the type_info class directly, 
//because the class has only a private copy constructor. The only way to construct a (temporary) type_info 
//object is to use the typeid operator. Since the assignment operator is also private, you cannot copy or assign 
//objects of class type_info.


#include<iostream>
#include<typeinfo>//不必包含这个头文件也可以
using namespace std;
class lei
{
public:
	typedef double type;
	class type2
	{
	public:
		void show(){cout<<"show called in lei::type2"<<endl;}
	};
};
/////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class leiA
{
public:
	typedef T type;
	void showLeiXin(){cout<<typeid(T).name()<<endl;}
	//leiA(){ti=new type_info(typeid(T));}//type_info只可以由关键字typeid来得到
	//~leiA(){delete ti;}
	//type_info *ti;
};
int main()
{
	cout<<"---------------------------------------------"<<endl;
	{
		cout<<"=>用类内类型声明变量"<<endl;
		lei a;
		lei::type b=3;
		cout<<b<<endl;
		lei::type2 c;
		c.show();
	}
	cout<<"---------------------------------------------"<<endl;
	{
		cout<<"=>使用typeid判断模板类生成的具体类型"<<endl;
		leiA<double> a;
		a.showLeiXin();
		cout<<typeid(char).name()<<endl;
		lei b;
		cout<<typeid(b).name()<<endl;
		cout<<typeid(lei::type).name()<<endl;
		cout<<typeid(lei::type2).name()<<endl;
		cout<<typeid(typeid(double)).name()<<endl;
	}
	return 0;
};