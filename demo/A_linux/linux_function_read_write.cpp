#include<iostream>
#include<cstdio>
#include<arpa/inet.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>

#include<stdlib.h>
#include<dirent.h>
using namespace std;
int main(int argc,char*argv[])
{

	char buf[1024];
	int n=0;
	if(n=read(0,buf,sizeof(buf))>0)
	{
		if(write(1,buf,n)==-1)
		{
			cout<<"call write error"<<endl;
			return -1;
		}
	}
	return 0;
}
	
