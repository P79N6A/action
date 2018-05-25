//1参数初始化列表中的顺序可以改变，形参顺序、列表中顺序、声明顺序可以各不相同。但是数据真正地被赋值初始化
//是按声明顺序走的。
//2类中定义引用类型，普通const可以在参数初始化列表中初始化。
//3在参数初始化列表中初始化，比在构造函数内部初始化效率要高。
#include<iostream>
using namespace std;
class leiA
{
public:
	leiA(char ch='A',double dou=3.14,int in=100):b(dou),c(ch),a(in){}
	int a;
	double b;
	char c;
};
int main()
{
	{
		leiA a('Z',2.718,10);
		cout<<a.a<<endl
			<<a.b<<endl
			<<a.c<<endl;
	}
	return 0;
}