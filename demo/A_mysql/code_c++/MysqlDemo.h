

#ifndef MYSQL_COMMON_H_
#define MYSQL_COMMON_H_


#include<my_global.h>
#include<my_sys.h>
#include<mysql.h>
namespace ns {
class MysqlQuery
{
public:
	bool	is_connected;
	MYSQL* conn;
	MYSQL_RES*	result;
	MysqlQuery();
	~MysqlQuery();
	bool	query(const char*qstr,bool want_result=true);
};

} /* namespace ns */

#endif /* MYSQL_COMMON_H_ */
