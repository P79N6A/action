

#include<stdlib.h>
#include"MysqlDemo.h"

namespace ns {

MysqlQuery::MysqlQuery()
{
	is_connected=false;
	conn=0;
	result=0;
	conn=mysql_init(NULL);
	if(!conn)
	{
		LOG_ERROR("mysql_init error");
		return;
	}
	char*unix_socket;
	if(strlen(CONFIG.stMysql_config.szUnix_socket)==0
			|| strncmp(CONFIG.stMysql_config.szUnix_socket,"NULL",sizeof(CONFIG.stMysql_config.szUnix_socket))==0
			|| strncmp(CONFIG.stMysql_config.szUnix_socket,"0",sizeof(CONFIG.stMysql_config.szUnix_socket))==0)
	{
		unix_socket=0;
	}
	else
	{
		unix_socket=CONFIG.stMysql_config.szUnix_socket;
	}
	if(!mysql_real_connect(
			conn,
			CONFIG.stMysql_config.szHost,
			CONFIG.stMysql_config.szUser,
			CONFIG.stMysql_config.szPasswd,
			CONFIG.stMysql_config.szDatabase,
			CONFIG.stMysql_config.dwPort,
			unix_socket,
			CONFIG.stMysql_config.dwClient_flag))
	{
		LOG_ERROR("mysql_real_connect error[%u]%s",mysql_errno(conn),mysql_error(conn));
		return;
	}
	is_connected=true;
}
MysqlQuery::~MysqlQuery()
{
	if(result)
	{
		mysql_free_result(result);
		result=0;
	}
	if(conn)
	{
		mysql_close(conn);
		conn=0;
	}
}
bool	MysqlQuery::query(const char*qstr,bool want_result)
{
	if(mysql_query(conn,qstr))
	{
		LOG_ERROR("mysql_query error[%u]%s",mysql_errno(conn),mysql_error(conn));
		return false;
	}
	if(want_result)
	{
		if(result)
		{
			mysql_free_result(result);
			result=0;
		}
		result=mysql_store_result(conn);
		if(!result)
		{
			LOG_ERROR("mysql_store_result error[%u]%s",mysql_errno(conn),mysql_error(conn));
			return false;
		}
	}
	return true;
}

} /* namespace ns */


















