//1模板函数的实现与声明必须在一起，否则会出错：无法解析的符号,目前大多数编译器不支持模板声明和实现的分离编译；
//2模板不会在头文件中编译，它只是在实例化时编译
//3函数模板的定义和专用化定义只能在全局，命名空间或类范围。实例化可以在全局。(实例化书上说也可以局部，但是实践不行)
#if 1
#include<iostream>
using namespace std;
template<typename T>
T fun(const T a)
{
	return a+10;
}
//template<> double* fun<double*>(const double* a)//该行编译出错
template<> double* fun<double*>(double*const a)//原模板中加const，相当于变量a是一个常量，同理
//double*定义的是一个指针，要求这个指针是一个常量而不是指针所指的内容（参见测试
//程序：函数重载）,所以要定义一个常指针才可以
//template<> double* fun(double*const a)//与上一个等价
{
	double*b=new double(*a+100);
	return b;
}
template int fun<int>(const int a);//
int main()
{
	{
		//template<> int fun(const int a){} //error C2951: 模板 声明只能在全局、命名空间或类范围内使用
		//template char fun<char>(const char a);//C++98和C++11均编译出错
		//书上解释在main()中可以实例化，但是这里不可以，不知为什么《C++Primer Plus》6thP430
	}
	{
		double pi=3.14;
		double *p=0;
		cout<<fun(pi)<<endl;
		cout<<*(p=fun(&pi))<<endl;delete p;
	}
	return 0;
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#if 0
//struct job
//{
//	char name[40];
//	double salary;
//	int floor;
//};
//template <class T>
//void Swap (T &, T &); // template prototype
//template <> void Swap<job>(job &, job &); // explicit specialization for job
//int main(void)
//{
//	template void Swap<char>(char &, char &); // explicit instantiation for char
//	short a, b;
//	//...
//	Swap(a,b); // implicit template instantiation for short
//	job n, m;
//	//...
//	Swap(n, m); // use explicit specialization for job
//	char g, h;
//	//...
//	Swap(g, h); // use explicit template instantiation for char
//	//...
//}
//template <typename T>
//void Swap(T &a, T &b) // general version
//{
//	T temp;
//	temp = a;
//	a = b;
//	b = temp;
//}
//// swaps just the salary and floor fields of a job structure
//template <> void Swap<job>(job &j1, job &j2) // specialization
//{
//	double t1;
//	int t2;
//	t1 = j1.salary;
//	j1.salary = j2.salary;
//	j2.salary = t1;
//	t2 = j1.floor;
//	j1.floor = j2.floor;
//	j2.floor = t2;
//}
//#endif