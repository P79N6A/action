//1将类的构造函数参数与operator()的参数一致，不会产生调用歧义，因为构造函数发生在构造阶段，
//由类名构造，而operator()发生在使用阶段，由对象名使用
//
#include<iostream>
using namespace std;
class lei
{
public:
	lei(int i){num=i;}
	int& operator()(int i){return a[i];}
	int a[10];
	int num;
};
int main()
{
	{
		lei a(2);
		a(1)=2;
		cout<<a(1)<<endl;
	}
	return 0;
}