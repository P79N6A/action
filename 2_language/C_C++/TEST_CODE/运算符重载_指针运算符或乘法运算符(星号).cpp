//1pointer-to-members operator".*"不能被重载，而指针运算符或说是乘法运算符"*"可以
//被重载
//2如果要把对象名当成一个指针来使用，可以将operator*重载或定义为类型转换函数。下面例子中的两种写法实质完全不一样，都可以实现相同的功能，第一个*是真正的返回一个地址，实际使用的*与它没有关系，而第二种写法中，*就是表示一个运算符，实际使用时的*号就是它
#include<iostream>
using namespace std;
class leiA
{
public:
	leiA(){a=0;e=2.71828;}
	operator double*(){cout<<"operator double*() called"<<endl;return &e;};
	double operator*(){cout<<"double operator*() called"<<endl;return e;};
	int *a;
	double e;
};
int main()
{
	cout<<"-----------------------------------------------------------------"<<endl;
	{
		leiA a;
		a.e=2.7;
		cout<<*a<<endl;
	}
	return 0;
}