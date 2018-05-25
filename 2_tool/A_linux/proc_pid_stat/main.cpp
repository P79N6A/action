#include<iostream>
#include<time.h>
#include<stdio.h>
#include<sys/time.h>
#include<arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<vector>
#include<map>
#include<string>
#include<unistd.h>
#include<stdlib.h>
#include<map>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
using namespace std;
class Lei
{
	int a;
	int b;
public:
	Lei()
	:a(111)
	,b(222){}
	void fun(){
		cout<<a<<' '<<b<<endl;
	}
};
Lei x;
FILE*FD=0;
void fun()
{
	pid_t pid=getpid();
//	cout<<pid<<endl;
	char buf[1024];
	snprintf(buf,sizeof(buf),"/proc/%d/stat",(int)pid);
//	cout<<"open file "<<buf<<endl;
	FD=fopen(buf,"r");
	if(!FD)
	{
		cout<<"open file error"<<endl;
		return ;
	}
	int v1;
	char v2[128];
	char v3;
	int v4,v5,v6,v7,v8;
	unsigned v9;
	unsigned long v10,v11,v12,v13,v14,v15;
	if(!fgets(buf,sizeof(buf),FD))
	{
		cout<<"fgets error"<<endl;
		return;
	}
	struct timeval tv;
	gettimeofday(&tv,0);
	{
		static int last=0;
		unsigned now=tv.tv_sec*1000+tv.tv_usec/1000;
		if(now-last>=1000)
		{
			last=now;
				
	cout<<"read buf:"<<buf<<endl;
	sscanf(buf,"%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu",&v1,v2,&v3,&v4,&v5,&v6,&v7,&v8,&v9,&v10,&v11,&v12,&v13,&v14,&v15);
	cout<<v14<<'\t'<<v15<<'\t'<<sysconf(_SC_CLK_TCK)<<endl;
	cout<<((v14+v15)*10/1000)<<endl;
			static unsigned long last_tic=(v14+v15)*10;
			unsigned long now_tic=(v14+v15)*10;
			
			cout<<"-----"<<((now_tic-last_tic)*100/1000)<<"%"<<endl;
			last_tic=now_tic;
		}
	}
		
	fclose(FD);
}

int main(int argc,char*argv[])
{
	
	
	int last_time=time(0);
	while(true)
	{
		int now=time(0);
		if(now-last_time>=0)
		{
			last_time=now;

			fun();
		}
		usleep(100);
	}
    return 0;
}
