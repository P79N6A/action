//1类对于枚举，静态变量和静态常量维持一个公共的存储空间（这一点从使用sizeof测试出），所以枚举和静态常量，静态变量均在
//类的公共存储区存放。类中可以定义const 数据成员，只能在参数初化列表中初始化。见《C++ Primer Plus》6th P683
//最特别的就是类中普通const常量不能在声明时初始化，因为它只能在创建时初始化，也不能在构造函数内部
//初始化，因为在构造函数调用的时候，对象已经被创建好了，只是起到一个对象初始化的作用，而此时const
//变量不能被赋值，故只能在参数初始化列表中初始化。在C++11标准中，const普通成员可以在定义时初始化。
//1_1如果类中定义了引用类型，只能在参数初始化列表中初始化,因为只能在创建时初始化，而不能定义好再赋值。
//2枚举和静态常量均属于公共常量数据，常量数据在声明时就要初始化，所以在类的声明中可以初始化，要不也没
//机会了。而普通静态变量则可以在运算时赋值，所以把它当作一个变量来看，在类中定义的静态变量和普通变量
//一样，不能在声明时初始化，而要在构造函数，全局或函数中赋值。
//3类定义仅仅是作为编译器分配存储空间的一种描述和声明。所有常量在类声明中就可以初始化，而非const变量则
//则和普通数据成员一样的赋值规则。
//4静态非常量类成员不能在头文件中初始化（除构造函数），否则如果该头文件在多个cpp中被包含，静态变量
///会被认为是重复定义
//5如果把类中的数据成员和成员函数都定义为static，那么类声明就相当于一个对象，即使不定义对象，静态函数
//同样可以操作静态数据。静态数据成员是为了在不定义具体对象的情况下操作静态数据的方法。
#include<iostream>
using namespace std;
class leiA
{
public:
	leiA():f(10){}
	//const int a=1;编译出错：error C2864: “leiA::a”: 只有静态常量整型数据成员才可以在类中初始化
	//在C++11中可以被初始化。
	enum b{ba=2,bb=3};//枚举类型可以直接在类中初始化
	static const int c=3;
	//static int d=4;//error C2864: “leiA::d”: 只有静态常量整型数据成员才可以在类中初始化
	static int e;
	const int f;//类中const数据成员只能在参数初始化列表中初始化
};
int leiA::e=5;
class leiB
{
public:
	leiB(){e=10;}
	static const int c=3;
	static int e;
};
class leiC
{
	int a;};
class leiD
{};
int main()
{
	{
		leiA a,b;
		//a.c=5;//error C3892: “c”: 不能给常量赋值
		cout<<b.c<<endl;
	}
	cout<<"-------------------------------------------"<<endl;
	{
		cout<<"sizeof(leiA)="<<sizeof(leiA)<<endl;//只有f占长度
		cout<<"sizeof(leiB)="<<sizeof(leiB)<<endl;
		cout<<"sizeof(leiC)="<<sizeof(leiC)<<endl;
		cout<<"sizeof(leiD)="<<sizeof(leiD)<<endl;
	}
	return 0;
}


