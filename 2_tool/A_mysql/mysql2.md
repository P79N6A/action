
mysql字符集
===========================================
启动
----------------------------------
service mysqld start;stop;restart;

查看字符集
----------------------------------
show variables like 'character%';

设置字符集
----------------------------------
###1.最简单的修改方法，就是修改mysql的my.ini文件中的字符集键值，如    
	[client]
		default-character-set = utf8
	[mysqld]
		character_set_server =  utf8
修改完后，重启mysql的服务。
配置文件里的default-character-set相当于对命令行中对character_set_client、character_set_connection和character_set_results进行SET。

###2.还有一种修改字符集的方法，就是使用mysql的命令
     mysql> SET character_set_client = utf8 ;
     mysql> SET character_set_connection = utf8 ;
     mysql> SET character_set_database = utf8 ;
     mysql> SET character_set_results = utf8 ;
     mysql> SET character_set_server = utf8 ;
     mysql> SET collation_connection = utf8 ;
     mysql> SET collation_database = utf8 ;
     mysql> SET collation_server = utf8 ;
 
设置了表的默认字符集为utf8并且通过UTF-8编码发送查询，存入数据库的仍然是乱码。那connection连接层上可能出了问题。解决方法是在发送查询前执行一下下面这句： SET NAMES 'utf8';它相当于下面的三句指令：
SET character_set_client = utf8;
SET character_set_results = utf8;
SET character_set_connection = utf8;

如果连接软件的显示与数据库均设置为了utf8，select数据库显示仍然是中文乱码，那么是mysql客户端不支持中文显示的原因；