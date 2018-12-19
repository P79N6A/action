#include<iostream>
#include<WinSock2.h>
#include<ws2tcpip.h>
#include<stdio.h>
#include<string.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
int main()
{
	WSADATA wsaData;
	int ret=WSAStartup(MAKEWORD(2,2),&wsaData);
	if(ret!=0)
	{
		printf("WSAStartup error[%d]\n",ret);
		return -1;
	}
	struct addrinfo* result;
	//char host[]="zt.qq.com";
	//char host[]="www.163.com";
	//char host[]="www.baidu.com";
	//char host[]="www.google.com.hk";
//	char host[]="web-proxyhk.oa.com";
	char host[]="www.juhe.cn";
	//ret=getaddrinfo("https://www.google.com.hk","80",0,&result);
	//ret=getaddrinfo("gcloudvoice.apollo.qq.com","19988",0,&result);
	//ret=getaddrinfo("www.163.com","80",0,&result);
	ret=getaddrinfo(host,"https",0,&result);
	
	if(ret)
	{
		printf("getaddrinfo error[%d]\n",ret);
		return -1;
	}
	
	int sfd=socket(result->ai_family,result->ai_socktype,0);
	if(sfd==-1)
	{
		printf("create socket error[%d]\n",GetLastError());
		return -1;
	}
	if(connect(sfd,result->ai_addr,result->ai_addrlen)==-1)
	{
		printf("connect error[%d]\n",GetLastError());
		return -1;
	}
	{
		struct sockaddr_in* si=(struct sockaddr_in*)result->ai_addr;
		printf("connect ip:%s port:%hu\n",inet_ntoa(si->sin_addr),ntohs(si->sin_port));
	}
	
	
	string str;
	str+="GET / HTTP/1.1\r\n";
	str+="Host: ";
	str+=host;str+="\r\n";
	str+="Connection: close\r\n";
	str+="\r\n";
	if(send(sfd,str.c_str(),str.size(),0)==-1)
	{
		printf("send error[%d]\n",GetLastError());
		return -1;
	}

	struct sockaddr_in cli_addr;
	char buf[2048];
	int len=0;
	int n=1;
	while(n>0)
	{
		n=recv(sfd,buf+len,sizeof(buf)-len,0);
		len+=n;
	}
	if(n==-1)
	{
		printf("recv from error[%d]\n",GetLastError());
		return -1;
	}
	printf("recv content-------------------------------------\n%s",buf);
	
	closesocket(sfd);
	freeaddrinfo(result);
	WSACleanup();
	return 0;
}