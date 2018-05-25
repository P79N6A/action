//1构造函数不能由对象显式调用，而析构函数可以。
#include<iostream>
using namespace std;
class leiA
{
public:
	leiA(int n=0){num=n;}
	~leiA(){cout<<"~leiA() called"<<endl;}
	int num;
};
int main()
{
	{
		leiA a;

		//a.leiA(3);//不能由对象显式调用构造函数
	}
	return 0;
}