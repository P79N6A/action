//1重载赋值运算符，返回值最好写成引用类型，否则返回的是一个临时变量，不能写成连等的形式;
#include<iostream>
using namespace std;
class A
{
public:
	A(int i){a=i;}
	A operator=(const A&temp){a=temp.a;return *this;}
	int a;
};
int main()
{
	A x(1),y(2);
	cout<<(y=x).a<<endl;
	cout<<y.a<<endl;
	//cout<<y=x=y<<endl;//写成连等形式会发生错误

	return 0;
}