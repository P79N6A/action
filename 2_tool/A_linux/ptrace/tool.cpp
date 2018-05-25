#include<unistd.h>
#include<stdlib.h>
#include<iostream>
#include<sys/ptrace.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
using namespace std;
int main(int argc,char*argv[])
{
	if(argc!=4)
	{
		cout<<"wrong argument num"<<endl
			<<"./exe pid addr value"<<endl;
		return -1;
	}
	long aim=atol(argv[1]);
	unsigned long long addr=strtoull(argv[2],(char**)0,16);
	long value=atol(argv[3]);
	cout<<"aim program's pid is "<<aim<<" addr is "<<hex<<addr<<' '<<addr<<endl;
	cout<<"addr octal is "<<oct<<addr<<endl;
	cout<<"value octal is "<<oct<<value<<endl;
	if(-1==ptrace(PTRACE_ATTACH,aim,0,0))
	{
		cout<<"attach error"<<endl;
		return -1;
	}
//	usleep(1000000);
	int status=0;
	waitpid(aim,&status,0);

	long old_data=ptrace(PTRACE_PEEKDATA,aim,(void*)addr,(void*)0);
	if(errno)
	{
		cout<<"peek data error"<<errno<<" "<<strerror(errno)<<endl;
		return -1;
	}
	cout<<"old_data is "<<old_data<<endl;

	//if(-1==ptrace(PTRACE_POKEDATA,aim,addr,(void*)&value))
	if(-1==ptrace(PTRACE_POKEDATA,aim,addr,(void*)value))
	{
		cout<<"poke data error "<<errno<<" "<<strerror(errno)<<endl;
		return -1;
	}

	unsigned long long now_data=ptrace(PTRACE_PEEKDATA,aim,(void*)addr,(void*)0);
	if(errno)
	{
		cout<<"peek data error"<<errno<<" "<<strerror(errno)<<endl;
		return -1;
	}
	cout<<"modify "<<old_data<<" to "<<now_data<<endl;

	if(-1==ptrace(PTRACE_DETACH,aim,0,0))
	{
		cout<<"detach error  "<<errno<<" "<<strerror(errno)<<endl;
		return -1;
	}
		
	return 0;
}

