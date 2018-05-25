//1转换构造函数（一个参数的构造函数）和类型转换函数是一对互为逆过程的函数。都可以以
//explicit来区分被显式调用还是隐式调用。转换构造函数负责将其它类型转换为本类，而类型
//转换函数负责将本类转换为其它类型。一如在内置基本类型中，int和double在函数调用时
//可以隐式或显式相互转换以适应不同的参数类型需求。在引入类之后，通过定义其它类型
//转化为本类的构造函数和本类转换为其它类型 的类型转换函数，这些转换方向可以使得
//一个类对象在不同的场合体现不同的属性，像变色龙一样来适应不同的参数类型需求。
//在不同需求下担任不同的角色。
//2如果要引入leiA到leiB的转换，一种方法在leiA中定义类型转换函数operator leiB(){……}，
//一种是在leiB中定义构造函数leiB(const leiA&){……}。不能两种都定义，否则会出现
//调用歧义。
//3过多地定义不同类型间多种的隐式类型转换会使得出现模糊调用，有时编译器无法决定
//调用哪个，有时编译器会自动选择一个而不显示bug。所以定义类型转换时最好使用
//explicit将一个方向的转换定为显式转换，避免两个类在任何情况下都相互转换替代的情况。
//4类型转换函数除了可以转换成内置类型，还可以转换为其它类类型，如operator lei(){}
//5类是一种非常好的打包方法，它可以将任何类型，包括对内置基本数据类型，类类型，
//函数类型，数组类型，任何重组和打包类型等进行进一步组合打包。通过恰当的打包
//和定义，一个类对象可以像普通变量一样运算（基本运算符重载），像函数一样调用
//（重载()运算符），像数组一样使用如a[i]，以及像任何想像中的抽象使用方法一样
//使用，它很灵活。而类的类型转换函数和转换构造函数则使得一个类对象可以在恰当
//时候转换为其它类型，也可以将其它类型转换为它，因为程序设计中为使得一个类对象
//的干净清晰，不会为一个类对象打包所有的特性。转换构造函数和类型转换函数，
//为新特性的增加提供了很简练的方法。
//6转换构造函数和类型转换函数本质是将一种资源整合成另一种资源的方式，它是一种
//对已存在类型所包含数据整合到另一个类型内容的方法。类是一种打包手段，类型转换
//是对已经有类型的一种资源整合传递手段。这种整合传递功能是一种函数，当然可以
//明确地定义一种函数来实现这种转化，可以说这两种转换就是一种函数而已。
//但这样做增加了编写函数和使用的麻烦，但是
//运行速度更快。而使用转换构造函数和类型转换函数则可以实现隐式转换或写法简单的
//显式转换，可以使代码看起来，
//写起来更加简单，但是也增加了复杂性，增加了类型误用的可能性。所以在程序设计中
//对于极其常用的转换使用隐式转换，而一般常用的转换则使用显式转换（可大大增加
//程序的可读性），不常用的直接定义成员函数来实现转换功能（可增加程序运行效率
//并大大增加可读性，因为编译器不用一一匹配合适的函数且名称可自己命名，如fun(string(leiob))
//如果定义的是隐式转换，且隐式调用，则函数fun使用为：fun(leiob),这时编译器会看
//leiob是否符合所有重载fun形参，如果不符合看有没有可用的转换，包括是否可通过
//构造函数以leiob为形参，把leiob转换为fun形参类对象；包括类型转换函数，
//也就是leiob是否存在可以转换为fun形参的类型转换函数；这个匹配过程会降低效率
//但如果为显式调用或定义为显式转换，使用时为fun(string(leiob)),则编译器直接寻找
//string类的构造函数看有没有符合条件的（如果fun还有以leiB为形参的，以leiC为形参的重载），
//，也会寻找看leiob有没有直接转换为string类型转换函数，而不用看有没有转换为leiB leiC
//的类型转换函数，如果定义为函数来实现这个功能，则直接为fun(leiob.toString())，
//编译器会直接在leiob中寻找这个函数完成相应的转换。简单明快。便于调试。），前
//面所说的效率都是指编译效率，编译成功后，程序已经明确知道要调用哪个，所以
//不影响运行效率，因为C++是静态语言（上面仅是自己的推断理解）。
//隐式转换可显式调用。
//转换可以定义在转换方的构造函数或者被转换方的类型转换函数。当然对于内置基本类型
//不可以定义这些个函数，可以定义在类中，它们之间的转换系统默认就有。
//7总之，这两种函数，使得同一个类，整合资源适应角色的能力很强。在实际编程中
//应该在定义构造类型，类型转换时应该加上explicit关键字，再根据实际需要再考虑
//去掉这个关键字。当一个类需要在不变的情况下既能转换为一种类型的一个值，又能
///转换为该类型的另一个值，只能使用定义函数来整合资源。
//8函数的作用就是整合资源，适应角色。
//9复制构造函数使得一个类对象在赋值时像另一个类型一样（其它类型向它赋值），
//而类型转换函数使得类对象在被用到时其内部内容时像另一种类型一样（它向
//其它类型赋值）。
//10类型转换可以定义为返回类成员或者类对象本身的指针。

#include<iostream>
#include<string>
#include<cctype>
using namespace std;
class leiA;
class lei
{
public:
	lei(double d=0){num=d;zj=d>0?true:false;}
	operator bool(){return zj;}
	operator char(){return 'C';}
	operator string(){return "lei";}
	operator leiA();
public:
	lei& set(double d)
	{num=d;
	if(d>0)zj=true;
	else zj=false;
	return *this;
	}
	void show(){cout<<"num:"<<num<<'\t'<<"bool:"<<zj<<endl;}
private:
	double num;
	bool zj;
};
class leiA
{
public:
	leiA(){}
	friend ostream&operator<<(ostream&os,leiA&cl);
	//{//也可以类内定义
	//	os<<"funciton operator<< called in leiA"<<endl;
	//	return os;
	//}
};
ostream&operator<<(ostream&os,const leiA&cl)
{
	os<<"funciton operator<< called in leiA"<<endl;
	return os;
}
class leiC;
class leiB
{
public:
	leiB(int in){num=in;}
	operator int(){return num;}
	operator leiC();
	int num;
};
class leiC
{
public:
	leiC(double d=0){num=d;}
	//leiC(const leiB&cl){num=cl.num;}
	double num;
};
leiB::operator leiC(){cout<<"operator leiC() called"<<endl;return leiC(num);}
lei::operator leiA()
{return leiA();}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class converseClass
{
public:
	converseClass(double d){dou=d;which=1;}
	converseClass(int i){in=i;which=2;}
	converseClass(char c){ch=c;which=3;}
	converseClass(leiC c){leic=c;which=4;}
	operator double(){return dou;}
	operator int(){return in;}
	operator char(){return ch;}
	operator leiC(){return leic;}
	int which;
//private:
	double dou;
	int in;
	char ch;
	leiC leic;
};
converseClass fun1(converseClass a)
{
	////依类型处理
	//if(a.which==1) a.dou*=2;
	//if(a.which==2) a.in*=2;
	//if(a.which==3) a.ch=toupper(a.ch);
	//if(a.which==4) {a.leic.num*=2;}
	//或者去掉if部分，统统处理一遍
	{
		a.dou*=3;
		 a.in*=3;
		a.ch=toupper(a.ch);
		a.leic.num*=3;
	}

	
	return a;
}
//template<typename T>
//class TconverseClass
//{
//public:
//	TconverseClass(T v){general=v;}
//	operator T(){return general;}
//private:
//	T general;
//};
//TconverseClass fun2(TconverseClass v)//在fun2中还是要判断T是什么类型，这个暂时不会，没继续编，有时间补上
int main()
{
	{
		lei a;
		if(a.set(10)) a.show();//a.set()返回的是一个类，但是可以类型转换为bool类型用于判断，也可以用于运算
		//a可以被当作不同类型进行运算，这正是多态的体现
		if(!a.set(-10)) a.show();
		if(!a.set(10).set(-7)) a.show();

		cout<<bool(a)<<endl;//输出bool类型
		cout<<char(a)<<endl;//输出字符
		cout<<string(a)<<endl;//输出string类型
		//cout<<leiA(a)<<endl;//将a类型转换为leiA，而leiA重载了operator<<，故可直接输出。
		cout<<leiA()<<endl;
	}
	cout<<"----------------------------------------------------"<<endl;
	{
		cout<<leiB(leiB(leiB(leiB(5)))).num<<endl;//定义了由int到leiB的构造函数和由leiB到int的类型转换函数
		//那么int类型和leiB在任何场合下可以相互转换替代
	}
	cout<<"----------------------------------------------------"<<endl;
	{
		leiC c;
		leiB b(7);
		c=b;
		cout<<"c.num:"<<c.num<<endl;
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	cout<<"----------------------------------------------------"<<endl;
	{
		double a=1.1;int b=2;char c='a';leiC d(4);
		cout<<(a=fun1(a))<<endl;
		cout<<int(fun1(b))<<endl;
		cout<<char(fun1(c))<<endl;
		cout<<(d=fun1(d)).num<<endl;

		//cout<<(a=fun2(a))<<endl;
		//cout<<int(fun2(b))<<endl;
		//cout<<char(fun2(c))<<endl;
		//cout<<(d=fun2(d)).num<<endl;
	}
	cout<<"----------------------------------------------------"<<endl;
	{
		cout<<"=>类型转换为一个指针"<<endl;
		class lei
		{
		public:
			lei(){a=10;}
			operator int*(){return &a;}
			int a;
		};
		lei a;
		cout<<*a<<endl;
		*a=5;
		cout<<a.a<<endl;
		int*p=&a.a;//定义一个指针指向对象的成员
		cout<<*p<<endl;
	};
	return 0;
}