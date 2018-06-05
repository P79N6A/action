

php日志处理
-------------------------------------------
在php.ini中搜error,在"Error handling and logging"模块，设置display_errors使得错误信息是否打印到屏幕，设置error_log使日志打印到文件，如果两个开关同时打开，则同一个动作的操作会分别打印到相应位置。


出现关于时间的warning时需要在php.ini中设置
-------------------------------------------
date.timezone = Asia/Chongqing


php打印输出变量
-------------------------------------------
echo
print_r		可以输出整个数据结构（如整个数组）
var_dump	可以输出整个数据结构中变量类型及数值
printf		类似C语言中printf
sprintf		与printf使用类似只是把结果返回到一个string中；



