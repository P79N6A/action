//1运算符重载时，包括定义的构造函数和类型转换函数一定不能使重载的运算符发生重载调用或者类型转换方面的歧义
//重载为友元函数时，类对象与内置类型的混合运算：内置+类、类加+内置，在定义双向类型转换的情况下会造成
//歧义，而重载为成员函数时，内置+类，不存在歧义，只能将类转化为内置，而在类+内置的情况下存在歧义
//2所以，如果将运算符重载存在内置类型和类对象的混合运算，那么类型转换只能定义单向的，而不能定义双向的，否
//则会引起歧义。如果只存在类对象与类对象之间的运算，或者内置类型与内置类型之间的运算，那么可以定义双向的类型
//转换。当然也可以用explicit关键字，把至少一个方向的类型转换定义为显式的，那么定义双向的类型转换也是可以的，
//这样一来不会存在歧义。（针对类型转换函数使用explicit关键字，是C++11标准中才加入的）。
//3综合2来看：存在混合运算，不能存在两个隐式转换，所以定义单向转换或者至少定义一个显式转换。
//            不存在混合运算，可以定义双向转换（可以是隐式或显式转换）
//4If you want to define an operator function so that the first operand is not
//a class object, you must use a friend function.Then you can pass the operands to the
//function definition in whichever order you want.
//4类型转换函数可以返回一个数的引用，编译没有错，但是这个引用不是真正的引用，不能通过引用
//赋值给其中的变量
#include<iostream>
using namespace std;
class lei
{
public:
	lei(double dou=1){daoshu=1/dou;}
	friend lei operator+(const lei&a,const lei&b);
	operator double(){return daoshu;}
	void show(){cout<<daoshu<<endl;}
	double daoshu;
};
lei operator+(const lei& a,const lei&b)
{
	lei temp;
	temp.daoshu=a.daoshu+b.daoshu;
	return temp;
}
	///////////////////////////////////////////////////////////////////////////////////////
class leiA
{
public:
	int in;
	operator int&(){return in;};
};
int main()
{
	{
		lei a,b,c;
		a=2;b=3;
		c=a+b;//正常类相加
		c.show();
		lei d;
		d=2+3;//加完之后再类型转换
		d.show();
		d=2+3.0;
		d.show();
	}
	cout<<"-------------------------------------------------"<<endl;
	{
		lei a,b;
		a=3;
		//b=a+2;//编译出错：error C2666: “operator +”: 2 个重载有相似的转换
//1>        e:\a_c++\test_code\运算符重载1.cpp(7): 可能是“lei operator +(const lei &,const lei &)”[通过使用参数相关的查找找到]
//1>        或“内置 C++ operator+(double, int)”
//1>        试图匹配参数列表“(lei, int)”时
		//出错的原因是：编译器不知道是把2转换为类进行类相加，还是把a转换成内置类型进行常规类型的相加
		//b.show();
	}
	cout<<"----------------------------------------------------"<<endl;
	{
		int a=3;double b,c=3.14;
		b=a+c;//不存在歧义，类比类对象的转换规则，它也应该存在将c转为int后与a相加和将a转为double后再与c相加的歧义
		//编译器是根据b的类型决定如何转换，所以只存在一次转换。同样的道理但类存在歧义，原因可能是类对象
		//在这两个歧义的运算结果中会产生很大的偏差，取决于转化函数的定义，调用过程会导致结果不同，
		//而内置对象以这两个歧义方法分别计算
		///得到的都是一个结果,调用过程可能歧义，但是结果一致，编译器可能是根据b的类型判断的。
		a=b+c;//不存在歧义，仅仅是将b与c相加完之后再赋值给a，出现了一个warning
	}
	cout<<"类型转换函数返回一个引用类型///////////////////////////////////////"<<endl;
	{
		leiA a;int b=10;
		//a=b;//error C2679: 二进制“=”: 没有找到接受“int”类型的右操作数的运算符(或没有可接受的转换)
		///g++:no match for 'operator=' in 'a = b'
		cout<<a.in<<endl;
	
	}
	return 0;

}