#include<iostream>
#include<stdlib.h>
using namespace std;

long long interval=5;
int main()
{
	while(true)
	{
		int now=time(0);
		static int last=0;
		if(abs(now-last)>=interval)
		{
			cout<<"hello world"<<endl;
			last=now;
			cout<<"interval="<<interval<<endl;
			cout<<&interval<<endl;
		}
	}

	return 0;
}
