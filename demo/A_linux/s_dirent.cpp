#include<iostream>
#include<cstdio>
#include<arpa/inet.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>

#include<stdlib.h>
#include<dirent.h>
using namespace std;
class B
{
public:
	int a;
	int b;
	~B(){cout<<"destructor of B"<<endl;}
};
class C
{
public:
	~C(){cout<<"destrctor of C"<<endl;}
	int d;
};
class A:public B
{
public:
	int c;
	C e;
	~A(){cout<<"destructor of A"<<endl;}
};
int main(int argc,char*argv[])
{

	DIR *dp;
	struct dirent*dirp;
	if(argc!=2)
	{
		cout<<"wrong number of arguments"<<endl;
		return -1;
	}
	if((dp=opendir(argv[1]))==0)
	{
		cout<<"can't open:"<<argv[1]<<endl;
		return -1;
	}
	while((dirp=readdir(dp)))
		cout<<dirp->d_name<<endl;
	closedir(dp);
	return 0;
}
	
