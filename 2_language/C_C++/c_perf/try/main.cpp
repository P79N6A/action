#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
using namespace std;
void fun(double*d)
{
	for(int i=0;i<100;i++)
	{
		(*d)*=2.8;	
//		usleep(1000);
	}
	return;
}
const char*fun_str(char*p)
{
	for(int i=0;i<1000;i++)
	{
		if(strcmp(p,"just do it")==0)
		{
			cout<<"good"<<endl;
		}
	}
	return p;
}
		
int main()
{
	double pi=3.14;
	fun(&pi);
	char a[]="len main";
	cout<<fun_str(&a[0])<<endl;
	printf("run  over\n");	
	return 0;
}
