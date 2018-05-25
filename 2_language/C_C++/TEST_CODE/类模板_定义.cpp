//1类模板参数可以是泛型，也可以是一个具体的已知类型和变量(已知类型和变量和形参定义类似)。
//如template<typename Type,int n>
//Expression arguments have some restrictions.An expression argument can be an integer
//type, an enumeration type, a reference, or a pointer.Thus, double m is ruled out, but
//double & rm and double * pm are allowed.Also the template code can’t alter the value
//of the argument or take its address.Also when you instantiate a template, the value used for the
//expression argument should be a constant expression.《C++ Primer Plus》6th P844

//2Although you can provide default values for class template type parameters, you can’t
//do so for function template parameters. However, you can provide default values for nontype
//parameters for both class and function templates.（这只是一个规定，我还不知为什么这样
//规定）

//3模板的专用化要用到template<>，模板的实例化要用到template主动构造或者在使用
//创建对象时构造
//4在类内对类外的一个函数专用化未通，这部分还未搞明白。
#include<iostream>
using namespace std;
template<typename T>
void fun(int i)
{
	T a;
	a=3;
	cout<<i<<'\t'<<a<<endl;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
template<typename T>
class K{};
template<typename T,int n=10>
class M{};
template<typename F>
void fun0(F x){cout<<"fun0 called"<<endl;}
template<typename F>
void fun1(F x){cout<<x<<endl;}

template<typename F>
void fun3(F x){cout<<"fun3 called"<<endl;}
template<typename F>
void fun4(F x){cout<<"fun4 called"<<endl;}
template<typename F>
void fun5(F x){}
template<typename F>
void fun6(F x){}
template<typename T>
void fun7(T& l);

template<
template<typename T>class A,typename C,int n,//模板参数类型可以是模板，普通类型，和非类型参数
template<typename T> class B=K,typename D=double,int m=8>//分别给默认参数的情况
class L:public M<C>//基类可以是模板
{
public:
	L(){num=100;}
	A<int> a;//分别定义成员的情况
	C ch;
	char str[n];
	B<double> b;
	D d;
	int arr[m];

	void fun_1(int a){cout<<"fun_1 called"<<endl;};//定义普通函数
	void fun_2(C c){cout<<"fun_2 called"<<endl;};

	//template void fun2(int x);// error C2252: 不能在当前范围内显式实例化模板
	//template void fun0(C c,B<int> b);//error C2252: 不能在当前范围内显式实例化模板
	//void fun0(C c);//不是类外的函数模板fun0的实例化（实例化要加template），
	//相当于声明了一个类内新函数
	template<typename X>
	void fun(X a,C b){cout<<"fun called"<<endl;};//将成员函数声明为函数模板，
	//一个是模板参数，一个在实例化时确定
	//template<>void fun1<C>(C x){};
	//可能A,B,C,D都是未确定的类型，所以不能用它们专用化
	//也可能专用化必须与函数模板在一个作用域内

	friend void fun2(L<A,C,n,B,D,m>& l);//一、Non-Template Friend Functions to Template Classes
	//在这里不能使用<>,因为fun2不是模板
	//在实际中使用到什么类型，就要在后面定义关于A,B,C,D,m,n的具体函数

    //friend void fun7(L&l); //error LNK2019: 无法解析的外部符号,L达是模板不是类
	friend void fun7<>(L<A,C,n,B,D,m>& l);//二、Bound Template Friend Functions to Template Classes
	//将普通函数定义为模板友元。其中的<>一定要加,The <> in the declarations identifies these as template specializations
	//friend void fun7<L<A,C,n,B,D,m> >(L<A,C,n,B,D,m>& l);//与上一个等价
	//这里的专用化并没有在前面加template<>，加上编译错误，我想这可能是因为模板的专用化
	//需要有具体的类型，而这个类本身就是模板，没有具体类型，所以这个专用化不是正规的
	//专用化，故只在后面加了<>
	template<typename U,typename V>//三、Unbound Template Friend Functions to Template Classes
	friend void fun8(U a,V b);
private:
	int num;
};
void fun2(L<K,char,5,K,double,7> x){}
template<typename T>
void fun7(T& l)
{cout<<"fun7 called with:"<<l.num<<endl;}

template<typename U,typename V>
void fun8(U a,V b){}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	fun<double>(3);//如果不显式调用fun无法知道模板参数是什么
	cout<<"============================================="<<endl;
	L<K,char,5,K,double,7> a;
	a.fun(1,'c');
	a.fun_1(1);
	a.fun_2('c');

	fun7(a);
	//a.fun0('c');////检测调用的是类内fun0还是类外fun0
	//a.fun3();//error C2039: “fun3”: 不是“L<A,C,n,B,D,m>”的成员
	//a.fun7();
	return 0;
}