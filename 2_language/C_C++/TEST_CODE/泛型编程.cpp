//1类的运算符重载和类型转换（包括构造转换函数和类型转换函数）
//为C++泛型编程打下坚实的基础，它使得类使用起来能够向基础类型一样简单
//但同时又可以实现通过其它运算符重载和自定义函数实现更复杂的功能。
//比如在模板编程中可以更好地实现类对象和基础类型的统一编程。
//2如果几个类型需要同样的操作步骤或方法，那么完全可以将这个方法
//定义成一个泛型处理方法，比如模板函数，模板类等。并且结合相应
//的操作为这些类型定义相同的接口（可以重载相同的运算符或者
//使用相同的函数名）。如果不使用模板可以将它们类型转换为一个
//统一的标准类（相当于做了一次包装改造），然后对这个标准类型
//进行处理。
//3类型转换和模板是两种基本泛型编程理念（自己的理解）。而运算符
//重载则是统一与基础类型的运算规则，简化类的操作的有效方法。
//运算符重载改变的是函数操作形式，而不改变操作方法。类型转换和模板
//结合使用可起到更好的效果。类型转换类型宽度把持窄，类型转换属于
//一种个性化修饰和服务，而模板类型范围要宽，一视同仁地服务。
//4模板实例化的生成不会考虑类型转换。但在模板内部函数可以有
//类型的自动转换。
#include<iostream>
using namespace std;
class leiA
{
public:
	leiA(int n):num(n){}
	operator int(){return num;}
	//explicit operator int(){return num;}
	int num;
};
//class leiB
//{
//public:
//	leiB(int n):num(n){}
//	int operator-(int i){return num-i;}
//	int num;
//};
template<typename T1,typename T2>
int operator+(T1 t1,T2 t2)//这里的operator只是改变的函数调用形式
{
	T1 r;
	cout<<"function operator+ called"<<endl;
	//r=t1+t2;//在模板内部不会自动类型转换，而会造成递归调用。
	//运行1+leiA(2)时循环调用operator+函数后崩溃：warning C4717: “operator+<int,leiA>”: 
	//如递归所有控件路径，函数将导致运行时堆栈溢出
	//若二问题：其一：1可以被构造为一个类对象，
	//但是这个类对象没有重载+，不通，但是leiA(2)可以类型转换为一个int型，
	//int型本身就有一个opterator +，没必要调用自定义operator+函数，
	//但编译器是优先寻找类型匹配的函数，所以一定会调用自定义的operator+
	//其二：系统调用了自定义operator+函数，但是在函数内部t1+t2本身又在不需要
	//类型转换的情况下满足本函数的调用需求，所以它会递归无限循环
	//调用这个函数而没有结果。解决办法将函数内部的+使用显式类型转换
	r=t1+int(t2);
	//r=t1*t2;//如果定义了operator+会自动调用类型转换，不会造成递归调用。
	return r;
}
template<typename T>
int operator-(T a,T b)
{
	cout<<"function operator- called"<<endl;
	//return a-b;//与函数本身调用形式和类型一样，会造成递归调用
	return leiA(a).num-b;
};
template<typename T>
int fun1(T t)
{
	return int(t);
}
int main()
{
	cout<<"------------------------------------------------------"<<endl;
	{
		cout<<"=>模板实例化生成不会考虑类型转换"<<endl;
		cout<<1+leiA(2)<<endl;
		cout<<20-leiA(10)<<endl;//并没有调用function operaotr-，因为leiA(10)类型转换
		//为基础类型后，基础类型使用自己的operator-。
		//同时，如果编译器把20自动类型转换为leiA(20)，同样可以套用模板
		//但是实际运行没有。把类型转换改为显式后，编译器找不到类型
		//匹配的函数。这说明，模板对于具体函数的生成不会考虑类型转换
		//而是实打实的看使用的类型。
		cout<<leiA(10)-20<<endl;//同上，编译器将类类型转换为基础类型
		cout<<leiA(10)-leiA(20)<<endl;//使用了模板
		//cout<<(operator-<leiA>(leiA(10),20))<<endl;//编译出错：可能对于运算重载不能显式指定类型
		cout<<fun1<leiA>(leiA(11))<<endl;//对模板函数实例化显式指定类型
	}
	return 0;
}
