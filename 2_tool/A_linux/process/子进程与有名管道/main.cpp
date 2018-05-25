#include<unistd.h>
#include<sys/stat.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<signal.h>
#include<iostream>
#include<string.h>

using namespace std;
const char FIFOA[]="./pipefileA";
const char FIFOB[]="./pipefileB";
const char *USER;
const char *IP;
const char *PORT;
const char *PASSWORD;
int main(int argc,char*argv[])
{
	signal(SIGTTOU,SIG_IGN);
	for(int i=0;i<argc;i++)
	{
		cout<<argv[i]<<endl;
	}
	if(argc<5)
	{
		cout<<"error wrong format(example: ./program user ip port password)"<<endl;
		return -1;
	}
	else
	{
		USER=argv[1];
		IP=argv[2];
		PORT=argv[3];
		PASSWORD=argv[4];
	}
	cout<<"hello_world"<<endl;
	if(access(FIFOA,F_OK)==-1)
	{
		cout<<"pipe file is not exist, so create it "<<endl;
		int fd=mkfifo(FIFOA,0777);
		if(fd==-1)
		{
			cout<<"mkfifo error"<<endl;
			return -1;
		}
	}
	if(access(FIFOB,F_OK)==-1)
	{
		cout<<"pipe file is not exist, so create it "<<endl;
		int fd=mkfifo(FIFOB,0777);
		if(fd==-1)
		{
			cout<<"mkfifo error"<<endl;
			return -1;
		}
	}
	if(fork()==0)
	{
		int fw=open(FIFOA,O_WRONLY);
		if(fw==-1)
		{
			cout<<"open error"<<endl;
			return -1;
		}
		if(dup2(fw,1)==-1)
		{
			cout<<"dup2 called error"<<endl;
			return -1;
		}
		if(dup2(fw,2)==-1)
		{
			cout<<"dup2 called error"<<endl;
			return -1;
		}
		int fr=open(FIFOB,O_RDONLY);
		if(fr==-1)
		{
			cout<<"open error"<<endl;
			return -1;
		}
		if(dup2(fr,0)==-1)
		{
			cout<<"dup2 called error"<<endl;
			return -1;
		}
		//execl("/bin/ls","ls",(char*)0);
		//execl("/bin/ls","ls","-l",(char*)0);
		//execl("/usr/bin/ssh","ssh","-tt","-l",USER,"-p",PORT,IP,(char*)0);	
		execl("/usr/bin/ssh","ssh","-l",USER,"-p",PORT,IP,(char*)0);	
		return 0;
	}
	else
	{
		int fr=open(FIFOA,O_RDONLY);
		if(fr==-1)
		{
			cout<<"open error"<<endl;
			return -1;
		}
		int fw=open(FIFOB,O_WRONLY);
		if(fw==-1)
		{
			cout<<"open error"<<endl;
			return -1;
		}
		char buf[20480];
		int len=0;
		int step=0;
		while(true)
		{
			int s=read(fr,buf+len,sizeof(buf)-len);
			if(s==-1)
			{
				cout<<"read error"<<endl;
				return -1;
			}
			len+=s;
			
			cout<<"{"<<(buf+len-s)<<"}";
			if(strstr(buf-step,"Are you sure you want to continue connecting (yes/no)"))
			{
				char msg[]="yes\r";
				int msg_len=strlen(msg);
				int N=0;
				while(N<msg_len)
				{
					int n=write(fw,msg+N,msg_len-N);
					if(n==-1)
					{
						cout<<"line:"<<__LINE__<<" write error:"<<errno<<endl;
						return -1;
					}
					N+=n;
				}
				cout<<"yes has been write"<<endl;
				step=len+10;
			}
			if(strstr(buf-step,"assword:"))
			{
				char msg[100];
				int msg_len=snprintf(msg,sizeof(msg),"%s\r",PASSWORD);
				int N=0;
				while(N<msg_len)
				{
					int n=write(fw,msg+N,msg_len-N);
					if(n==-1)
					{
						cout<<"line:"<<__LINE__<<" write error:"<<errno<<endl;
						return -1;
					}
					N+=n;
				}
				cout<<"password has been write"<<endl;
				step=len+5;
			
			}
			if(strstr(buf,"Authentication successful."))
				break;
		}
		if(dup2(0,fw)==-1)
		{
			cout<<"dup2 called error"<<endl;
			return -1;
		}
		if(dup2(fr,1)==-1)
		{
			cout<<"dup2 called error"<<endl;
			return -1;
		}
		wait(0);
		
	}
		
		
	
	
	return 0;
}
