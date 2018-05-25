//其实编译器把struct当成一个类来处理（在visual 2005经典入门那本书看到的），只不过
//sturct中所有的变量在不声明public或private的情况下，默认是public，而class则默认是private，
//区别仅此而已，其它方面struct和类是一样的
#include<iostream>
#include<string>
int main(void)
{
	using namespace std;
	cout<<"struct========================================================"<<endl;
	struct jiegou
	{
		char name[20];
		int age;
		void display()//结构体也可以定义函数
		{
			cout<<name<<" "<<age<<endl;
		}
	//private:
	//	string aim;
	};
	jiegou a={"zhangliang",23};//
	a.display();
	cout<<"class========================================================"<<endl;
	class lei
	{
	public:
		char name[20];
		int age;
	//private:
	//	string aim;
	};
	lei b={"zhangliang",24};//如果含有私有成员保护成员虚函数等情况下（vs编译说是聚合类
	//才有这样的默认构造函数），没有这样的构造函数，在只含公有成员，无函数的情况
	//下有这样默认的构造函数。（相当于提供了一个无参的和一个有参的构造函数）
	//lei b={"zhangliang",24,"success"};
	cout<<b.name<<" "<<b.age<<endl;
	return 0;
}
//聚合定义为：
//数组没有以下内容的类、结构和联合：
//构造函数私有或受保护的成员基类虚函数另外，
//Visual C++ 不允许在包含构造函数的聚合中使用数据类型。
//以下内容表示在某一类型上尝试聚合初始化时可能引发 C2552 的原因： 
//类型具有一个或多个用户定义的构造函数。
//类型具有一个或多个非静态的、私有数据成员。
//类型具有一个或多个虚函数。类型具有基类。
//类型是 ref 类或 CLR 接口。
//类型是标量 (int i = {};)类型具有其元素具有析构函数的非固定维数组（从零开始的数组）。