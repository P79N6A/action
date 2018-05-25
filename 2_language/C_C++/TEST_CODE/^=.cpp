#include<iostream>
using namespace std;
int main()
{
	enum{a,b};
	cout<<a<<endl;//枚举默认从0开始
	int c=0;
	for(int i=0;i<10;i++)
	{
		cout<<(c^=1)<<" ";
	}
	return 0;
}