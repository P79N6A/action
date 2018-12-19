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

	cout<<stdin<<endl;
	cout<<stdout<<endl;
	cout<<stderr<<endl;
	int c;
	while((c=getc(stdin))!=EOF)
	{
		if(putc(c,stdout)==EOF)
		{
			cout<<"putc EOF"<<endl;
			return -1;
		}
	}
	return 0;
}
	
