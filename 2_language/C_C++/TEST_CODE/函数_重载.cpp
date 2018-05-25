//1重载函数参数类型与实际调用不匹配时，可以发生类型转化，如果是numeric类型，那么
//系统会自动转换，如果是自定义类型，如类等，那么要定义类型转换函数。如果函数要
//求输入的类类型，如果定义了相应的构造函数，输入普通数值也是可以的
//2对一个类对象定义类型构造函数，和类型转换函数，可以使得，函数调用时的输入类型
//不限于定义类型，有函数模板的效果
//3函数重载中的function signatures不是指函数类型（返回值类型），而是形参类型，必须是数量
//或者类型不一致，引用和非引用不能区分调用，故不能用于函数重载。加与不加const也不能
//用于构造重载函数，除过const数组。如果对要调用的变量本身区分const和非const，那
//自然不能构造重载函数族，如下面中的fun6,fun9。如果是指向const类型的数组，而指针本身不
//用const区分，const是可以用来构造重载函数的，如下面的fun7,fun8。
//4对非数组定义指针时用到的const相当于对数据本身是否可以改变作了一个定义，从fun6-fun9
//可以看到定义常指针与定义常变量都不能重载函数，而定义常数组和非常数组可以构造重载
//函数
#include<iostream>
using namespace std;
void fun1(int);
void fun1(int in)
{
	cout<<"int fun1 callled:"<<in<<endl;
}
void fun1(double);
void fun1(double df)
{
	cout<<"double fun1 called:"<<df<<endl;
}
class lei
{
public:
	lei(int in){num=in;}
	operator double(){return num;};
	int num;
};
void fun2(lei cl)
{
	cout<<"fun2 called:"<<cl.num<<endl;
}
void fun3(double& in)
{
	cout<<"fun3 called:"<<in<<endl;
}
void fun4(const double&in)
{
	cout<<"fun4 called:"<<in<<endl;
}
void fun5(double dou)
{
	cout<<"fun5 called with double type:"<<dou<<endl;
}
void fun5(long lo)
{
	cout<<"fun5 called with long type:"<<lo<<endl;
}
void fun6(int in)
{
	cout<<"fun6 called without const:"<<in<<endl;
}
//void fun6(const int in)//编译出错，相当于重复定义fun6:error C2084: 函数“void fun6(int)”已有主体
//{
//	cout<<"fun6 called with const:"<<in<<endl;
//}
void fun7(const char *p)
{
	cout<<"fun7 called with const:"<<p<<endl;
}
void fun7(char *p)
{
	cout<<"fun7 called without const:"<<p<<endl;
}
void fun8(const int *p)
{
	cout<<"fun8 called with const:"<<p[0]<<endl;
}
void fun8(int *p)
{
	cout<<"fun8 called without const:"<<p[0]<<endl;
}
void fun9(int *const p)
{
	cout<<"fun9 called with const:"<<p[0]<<endl;
}
//void fun9(int *p)//编译出错，相当于重复定义fun9：error C2084: 函数“void fun9(int *const )”已有主体
//{
//	cout<<"fun9 called without const:"<<p[0]<<endl;
//}
int main()
{
	fun1(2);//2会被默认为int类型，如果没有定义输入参数为int的fun1函数，会调用输入类型为
	//double的fun1函数
	fun1(2.71828);//会调用输入类型为double的fun1函数，如果没有定义输入类型为double的fun1函数
	//会调用输入类型为int类型的fun1函数，这时会产生一个warning，提示类型转换引起的精度丢失。
	fun1(3l);
	//fun1(4F);//写成4F会出错，必须要带小数点
	fun1(4.1f);
	cout<<"-------------------------------------------------------------------------------"<<endl;
	{
		lei a(100);
		fun1(a);//虽然调用的是一个类，但是因为类中定义了类型转换函数，所以这个类会自动
		//转换为int类型，然后再调用函数
		fun2(a);//调用fun2，常规
		fun2(101);//函数会自动将int类型转换为lei，然后再使用fun2，因为定义了相应的构造函数
	}
	cout<<"-------------------------------------------------------------------------------"<<endl;
	//fun3(2.71828);//编译错误：“fun3”: 不能将参数 1 从“double”转换为“double &”,如果是变量就可以如下面
	//如果函数形参为引用类型，那么调用的实参为右值就会出错，必须把形参声明为const引用
	//函数才可以以右值作为形参
	{
		double dou=3.14;
		fun3(dou);
		fun4(2.71828);
	}
	cout<<"forth:---------------------------------------------------------------------------"<<endl;
	{
		//fun5(3);//编译出错： error C2668: “fun5”: 对重载函数的调用不明确
//1>        e:\a_c++\测试程序\函数重载.cpp(41): 可能是“void fun5(long)”
//1>        e:\a_c++\测试程序\函数重载.cpp(37): 或“void fun5(double)”
	   //编译器不知道自动转换为哪种类型，因为默认是int类型
		fun5(3.0f);//指定类型后调用
		fun5(5l);
	}
	cout<<"fifth:---------------------------------------------------------------------------"<<endl;
	{
		fun6(1111);
		const char p1[20] = "How's the weather?";
		char p2[20] = "How's business?";
		fun7(p1); // fun7(const char *);
		fun7(p2); //fun7(char *);
		const int a1[2]={1,2};
		int a2[2]={11,22};
		fun8(a1);//void fun8(const int *p)
		fun8(a2);//void fun8(int *p)
		//fun9(a1);//不能将非const转为const
		fun9(a2);
	}
	{
	//void sink(double & r1); // matches modifiable lvalue
	//void sank(const double & r2); // matches modifiable or const lvalue, rvalue
	//void sunk(double && r3); // matches rvalue
//	double x = 55.5;
//const double y = 32.0;
//stove(x); // calls stove(double &)
//stove(y); // calls stove(const double &)
//stove(x+y); // calls stove(double &&)
	}
	return 0;
}