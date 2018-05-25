#include<iostream>
#include<string>//在C++98标准的编译器中必须加上，而在C++11标准中不必加
//在两个标准中都可以在函数内部定义局部类，它相当于一个局部变量，只不过是自定义类型
using namespace std;
int main()
{
	void fun();
	void classfun();

	fun();
	classfun();
	//A b;b.display();//A是局部class，不可以在这用
	return 0;
}
void fun()
{
	struct st//可以定义局部struct
	{
		string str;
	};
	st as;
	as.str="struct";
	cout<<as.str<<endl;
}
void classfun()
{
	class A//可以定义局部class
	{
	public:
		A()
		{
			a="class";
		}
		void display()
		{cout<<a<<endl;}
	private:
		string a;
	};
	A a;
	a.display();
}
