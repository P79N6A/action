//1由于普通变量之间运算存在自动的类型转换，而在自定义类型中，构造函数和类型转换函数提供了，自定义类型和内置类型，
//自定义类型和自定义类型之间的自动类型转换方式，如果定义了这个函数，那么编译器会自动实别并自动进行类型转换
//自动类型转换是在如double与int相加时等等情况下发生
//2内置类型在运算时如果不能自动类型转换，编译器会报错，这时可根据需要使用强制类型转换
//3自定义类型不存在强制类型转换，内置类型的强制类型转换类型相当于一个运算符，这一点在类中定义类型转换函数时
//就可以看到，如operator double(){}，而自定义类型没有这样的运算符而且内部数据结构复杂，不存在强制类型转换可以理解 
//4自定义型除过函数等，如果是类，它虽然不存在强制类型转换，但是如果存在其强制类型转换的格式之一如：
//类型（obj）和(类型)obj，前一个同
//构造函数是一样的，后一种编译器视为前一种类型，所以如果定义了相应的构造函数，也就等价于存在强制类型转换
///5自动类型转换也称为隐式转换（implicit conversion）,强制类型转换也称为显式转换（explicit conversion）
//6当一个类既定义有向Int的类型转换，又定义有向double的类型转换,在进行隐式类型转换时就会造成ambiguity，所以
//必须使用显式转换或者是去掉一个转换方式
//7如果在类型转换函数或构造函数前面加explicit关键字，那么这些类型转换函数或者是构造函数都要显式调用 ，这样可以
//防止自动类型转换可能带来的隐含error
//8显式定义比隐式定义使程序编译速度要快，隐式转换要测试各种可能的转换
#include<iostream>
using namespace std;
class lei
{
public:
	lei(){d=0;}
	lei(double dou){d=dou;}
	operator double(){return d;}
	void show()
	{ 
		cout<<"d:"<<d<<endl;
	}
private:
	double d;
};
class leiB;
class leiA
{
public:
	leiA(){integer=1;}
	leiA(int in){integer=in;}
	//leiA(leiB b){integer=b.dou;}//编译出错：error C2027: 使用了未定义类型“leiB”
	//不能在一个类定义前使用它，而只能通过提前引用声明来用它声明一个原形，而不能使用；
	leiA(leiB);
	int integer;
	void show()
	{cout<<"leiA:"<<integer<<endl;}
};
class leiB
{
public:
	leiB(double d=3.14){dou=d;}
	leiB(leiA a){dou=a.integer;}
	double dou;
	void show()
	{cout<<"leiB:"<<dou<<endl;}
};
leiA::leiA(leiB b){integer=b.dou;}//由于用到了leiB的提前引用声明，所以函数定义只能写到类定义外面
int main()
{
	{
		lei a;
		a=3.14;//3.14通过构造函数自动类型转换为一个类然后赋值给a
		a.show();
		cout<<"cout<<a:"<<a<<endl;///自动类型转换之后再输出
		double b;
		b=a;//a通过类型转换函数被转成了double类型后赋值给b
		cout<<"b:"<<b<<endl;
		int c;
		c=a;//先把a转换成double再转换成int，当然如果在用Int也可以在类中直接定义operator int(){}
	}
	cout<<"---------------------------------------------------------------------------"<<endl;
	{
		leiA a;leiB b;
		a=3;
		(b=a).show();//因为定义了相应的构造函数，a会被自动类型转换为leiB然后再赋值给b
		b=4;
		(a=b).show();//b自动类型转换为leiA,转换方式如定义的构造函数
		a=leiA(b);
		a=(leiA)b;
		//a=lei(b);//没有定义相应的构造函数，也就不存在相应的类型转换
	}
	return 0;
}