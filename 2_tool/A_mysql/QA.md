
忘记密码或安装完mysql没有在日志中找到密码
--------------------------------------------------------------
可以通过重置密码来解决
https://dev.mysql.com/doc/refman/8.0/en/resetting-permissions.html
在高版本的mysql中可以通过ALTER USER 'root'@'localhost' IDENTIFIED BY 'MyNewPass';来设置密码，但在低版本（如5.1）需用SET PASSWORD FOR 'root'@'localhost' = PASSWORD('newpass');否则会出错；