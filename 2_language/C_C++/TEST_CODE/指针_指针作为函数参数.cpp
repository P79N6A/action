//1给指针赋值时，对左值才可以使用地址运算符
//2同不能为右值使用左值引用一样，不能对右值使用地址运算符，即使用指针（这个指针相当于左值指针）。
//3所有的指针操作只能针对左值而言，因为&运算符只对左值有效
#include<iostream>
using namespace std;
void fun1(int*p)
{
	cout<<"fun1 called with:"<<*p<<endl;
}
void fun2(int*const p)
{
	cout<<"fun2 called with:"<<*p<<endl;
}
void fun3(const int *p)
{
	cout<<"fun3 called with:"<<*p<<endl;
}
void fun4(const int*const p)
{
	cout<<"fun4 called with:"<<*p<<endl;
}
int main()
{
	{
		int a,*p;
		a=1;
		//p=&(a+10);//只有左值才可以使用地址运算符：error C2102: “&”要求左值
		fun1(&a);
		a=2;
		fun2(&a);
		fun3(&(a=3));
		fun4(&(a=4));
	}
	cout<<"------------------------------------------------"<<endl;
	{
		int a=10;
		//fun1(&(a+100));//编译出错： error C2102: “&”要求左值
		//由于&运算符必须要求左值，所以对于使用const是否可以让调用右值指针const数据的测试也就无法进行
		//（类比的是：const左值引用可以以右值为实参）
	}
	return 0;
}