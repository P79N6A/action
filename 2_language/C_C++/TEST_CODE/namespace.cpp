//1无名namespace相当于一个定义全局static变量的包
//2有名namespace必须使用using才能在一个region内使用
//3namespace内的变量可以初始化（我个人的理解是它和宏的作用是一样的），就像在使用
//using位置定义变量时可以初始化
//4在实际工作中最好将命名空间写在一个头文件中，使用起来方便，不用在每个cpp中都要
//写工作空间具体定义
//5使用标准头文件时，一定要使用using来包括某个namespace或某个变量
#include<iostream>
#include<string>
#include"namespace1.h"
using namespace std;
namespace test
{
	string name="ZhangLiang";//可以进行初始化，在使用命名空间的位置相当于把其中的定义直接
	//复制到那个位置
}
namespace//无名namespace 定义相当于一个定义全局static变量的空间
{
	int age=23;
}
namespace test2
{
	double pi=3.14;
}
namespace n3
{
	int v3=1;
}
using namespace test2;//相当于test2中的所有变量成为了全局extern变量
using test3::e;//将test3中的e作为全局变量
int quanju=123;
int quanju1=222;
void fun();
void fun1();
int main()
{
	age=2000;//无名namespace中的变量是全局的
	cout<<"age:"<<age<<endl;
	//name="happy";编译出错，未声明的标识符；必须运用using 来使用命名空间
	cout<<"pi:"<<pi<<endl;
	cout<<"test3::e:"<<e<<endl;

	fun();
	fun1();//在namespace2.cpp中定义


	//////////////////////////
	cout<<endl<<"test using namespace in { }"<<endl;
	{
		using namespace n3;
		v3=2;
		cout<<v3<<endl;
	}
	//v3=3;//this wil cause compile error, for v3's namespace has not been declared
	//cout<<v3<<endl;
	return 0;
}
void fun()
{
	using namespace test;//声明命名空间后才可使用
	cout<<name<<endl;
}
