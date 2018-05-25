#include<errno.h>
#include<string.h>
#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
using namespace std;
void mysignal(int x)
{
	cout<<"mysignal:"<<x<<"received by "<<pthread_self()<<endl;
}
void *tf1(void*arg)
{
	long id=(long)arg;
	//cout<<"thread:"<<id<<"start"<<endl;
//	while(true)usleep(10000000);
	usleep(1000000);
}
void *tf2(void*arg)
{
	long id=(long)arg;
//	cout<<"thread:"<<id<<"start"<<endl;
	//while(true)usleep(10000000);
	usleep(1000000);
}
int main(int argc,char*argv[])
{
	
	if(signal(SIGUSR1,mysignal)==SIG_ERR)
	{
		printf("signale called error[%d]%s\n",errno,strerror(errno));
		return -1;
	}	
	cout<<"parent thread is:"<<pthread_self()<<endl;
	pthread_t p1,p2;
	if(int ret=pthread_create(&p1,0,tf1,(void*)1))
	{
		cout<<"pthread_create 1 error:"<<ret<<endl;
		return -1;
	}
	cout<<"thread 1:"<<p1<<endl;
	if(int ret=pthread_create(&p2,0,tf2,(void*)2))
	{
		cout<<"pthread_create 2 error:"<<ret<<endl;
		return -1;
	}
	cout<<"thread 2:"<<p2<<endl;
	if(int ret=pthread_kill(p1,SIGUSR1))
	{
		cout<<"send kill "<<p1<<endl;
	}
	while(int ret=pthread_tryjoin_np(p1,0))
	{
		cout<<"pthread join error"<<ret<<endl;
	}
	while(int ret=pthread_tryjoin_np(p2,0))
	{
		cout<<"pthread join error"<<ret<<endl;
	}
//	if(int ret=pthread_join(p2,0))
//	{
//		cout<<"pthread join error"<<ret<<endl;
//	}
	//while(true)
	{
	//	cout<<"I'm "<<argv[0]<<endl;	
		usleep(2000000);
	}
	cout<<"has come out of while"<<endl;
	return 0;
}
