帮助
-------------------------------------------
help create event

打开事件功能：
-------------------------------------------
查看是否已打开：show variables like '%sc%';
开启：
SET GLOBAL event_scheduler = ON;
SET GLOBAL event_scheduler = 1;   — 0代表关闭

永久开启：
在my.cnf中的[mysqld]部分添加如下内容，然后重启mysql(mysql重启命令：service mysqld restart)
event_scheduler=ON


创建事件，这里举例每5秒钟定时删除table表中过期2分钟的数据：
-------------------------------------------
create event event_name on schedule every 5 second do delete from table_name where timestamp <  (CURRENT_TIMESTAMP() + INTERVAL -2 MINUTE);	命令中表不一定必须 是timestamp字段，只要是一个时间类型的字段就可以，如datetime；

如果事先存在这个事件可以使用如下命令进行删除：

drop event if exists event_name; 

查看存在的事件
-------------------------------------------
show events;

打开关闭事件
-------------------------------------------
开启事件：
alter event event_name on completion preserve enable;
关闭事件：
alter event event_name on completion preserve disable;