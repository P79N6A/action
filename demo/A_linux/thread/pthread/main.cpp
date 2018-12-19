#include<iostream>
#include<pthread.h>
#include<unistd.h>
using namespace std;

struct Arg
{
	const char *p;
	int x;
};
void* fun(void*arg)
{
	Arg*parg=(Arg*)arg;
	cout<<"funcalled:"<<parg->p<<endl;
	return arg;
}	
int main()
{
	
	pthread_t p;
	Arg arg;
	arg.p="the world change so quickly";
	
	if(int err=pthread_create(&p,0,fun,&arg))
	{
		cout<<"pthread_create error:"<<err<<endl;
	}	
//	sleep(5);
	int* retval;
	if(pthread_join(p,(void**)&retval))
	{
		cout<<"phtread_join error"<<endl;
		return -1;
	}
	cout<<"status:"<<*retval<<endl;
	return 0;
}
