//除数是0，编译通过，运行崩溃
#include<iostream>
using namespace std;
int main()
{
	int a=100,b=0,c=0;
	//cout<<a/b<<endl;//除数是0会直接导致运行程序崩溃，编译不会出错
	//计算中一定要对除数进行准备检查
	if(b)
		cout<<a/b<<endl;
	else
	{
		cout<<"除数是0，非正常退出"<<endl;
		exit(1);
	}
}