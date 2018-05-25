#include<iostream>
using namespace std;
static double pi=3.14;
int main()
{
	double pi=3.14*2;
	cout<<"全局pi："<<::pi<<endl;
	cout<<"main内："<<pi<<endl;
	{
		double pi=3.14*3;
		cout<<"全局pi："<<::pi<<endl;//调用的是全局的pi
		cout<<"{}内："<<pi<<endl;//调用这个当前block的pi
	}
	return 0;
}