//.1无论在同一地址改变一个const变量的值，这个值使用指针显示改变，而使用原变量一直
//是不变，但地址都一样，自己还不知编译器是怎么保护这个量的。
#include<iostream>
using namespace std;
int main()
{
	const int a=10;
	const int *p=&a;
	int *p1=const_cast<int*>(p);//去const修饰方法
	*p1+=10;
	cout<<a<<endl;
	cout<<*p1<<'\t'<<*p<<'\t'<<*(&a)<<endl;
	cout<<p1<<'\t'<<p<<'\t'<<&a<<endl;
	int *p2=reinterpret_cast<int*>(0x0012FF28);
	cout<<*p2<<endl;//这个地址的值确定被变为20了
	return 0;
}