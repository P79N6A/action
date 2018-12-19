#include<iostream>
#include<my_global.h>
#include<mysql.h>
#include<stdio.h>
#include<sys/time.h>
#include<string.h>
#include<sys/select.h>
#include<sys/types.h>
using namespace std;
unsigned int gettime_us()
{
	struct timeval tv;
	gettimeofday(&tv,0);
	return tv.tv_sec*1000000+tv.tv_usec;
}
int main()
{
	MYSQL* conn=mysql_init(NULL);
	if(!conn)
	{
		printf("init Error:%u: %s\n",mysql_errno(conn),mysql_error(conn));
		return -1;
	}
	if(!mysql_real_connect(conn,"localhost","root","","apollo_voice",0,0,0))
	{

		printf("connect Error:%u: %s\n",mysql_errno(conn),mysql_error(conn));
		return -1;
	}
	{
//		if(mysql_query(conn,"replace into apollo_voice_auth values(4000,'abc',now())"))
//		{
//			printf("query Error:%u: %s\n",mysql_errno(conn),mysql_error(conn));
//			return -1;
//		}
//		if(mysql_query(conn,"select * from apollo_voice_auth where business_id=4000"))
//		{
//			printf("query get error:%u: %s\n",mysql_errno(conn),mysql_error(conn));
//			return -1;
//		}
//		MYSQL_RES* result;
//		result=mysql_store_result(conn);
//		mysql_free_result(result);
//		if(mysql_query(conn,"select * from apollo_voice_auth where business_id=5000"))
//		{
//			printf("[%d]query get error:%u: %s\n",__LINE__,mysql_errno(conn),mysql_error(conn));
//			return -1;
//		}
//		result=mysql_store_result(conn);
//		if(!result)
//		{
//			printf("store result error [%u]:%s\n",mysql_errno(conn),mysql_error(conn));
//			return -1;
//		}
//		MYSQL_ROW row=mysql_fetch_row(result);
//		if(!row)
//		{
//			printf("fetch_row error result error [%u]:%s\n",mysql_errno(conn),mysql_error(conn));
//			return -1;
//		}
//			
//	      	int num_fields=mysql_num_fields(result);
//		for(int i=0;i<num_fields;i++)
//		{
//			printf("%s",row[i]);
//		}
	}	
	{
		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(conn->net.fd,&rfds);	
		char qs[200]="select * from apollo_voice_auth where business_id=4000";
		if(mysql_send_query(conn,qs,strlen(qs)))
		{
			printf("mysql_send_query error[%d]%s\n",mysql_errno(conn),mysql_error(conn));
			return -1;
		}
		printf("after send_query\n");

		MYSQL_RES* result;

		while(true)
		{
			static int n=0;
			printf("time:\t%d\n",n++);

			
		//	FD_SET(0,&rfds);	
			struct timeval tv;
			tv.tv_sec=1;
			tv.tv_usec=2;
			int retval=0;
			retval=select(1,&rfds,NULL,NULL,&tv);
			if(retval==-1)
			{
				printf("error occurred[%d]\n",errno);
				return -1;
			}
			printf("------------------------------------retval:%d\n",retval);
			if(retval==0)
			{
//				continue;
			}
			{
				static int n=0;
				printf("try receive data:%d\n",++n);
				if(mysql_read_query_result(conn))
				{
					printf("mysql_read_query_result error[%d]%s\n",mysql_errno(conn),mysql_error(conn));
					//
					continue;
				}
				result=mysql_store_result(conn);
				if(!result)
				{
					printf("store result error [%u]:%s\n",mysql_errno(conn),mysql_error(conn));
					return -1;
				}
				MYSQL_ROW row=mysql_fetch_row(result);
				if(!row)
				{
					printf("fetch_row error result error [%u]:%s\n",mysql_errno(conn),mysql_error(conn));
					return -1;
				}
					
	      			int num_fields=mysql_num_fields(result);
				for(int i=0;i<num_fields;i++)
				{
					printf("%s|",row[i]);
				}
				break;
			}
		}	

	}
	
	mysql_close(conn);
	printf("success closed\n");
	return 0;
		
}

