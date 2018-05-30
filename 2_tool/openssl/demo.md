
加密
------------------------------------------------------------------
echo -n 'abc' | openssl enc -base64	使用base64算法编码;
echo -n 'abc' | openssl enc -e -base64 -rc4 	使用rc4算法加密，密码通过命令行提示输入；如果没有-base64则会生成二进制数据；

echo -n 'abc' | openssl enc -e -base64 -rc4 -pass pass:password	使用命令行方式直接使用"password"加密abc；
echo -n 'abc' | openssl enc -e -base64 -rc4 -pass file:filename	打开文件filename并取第一行为密码

echo -n 'U2FsdGVkX1+9eA1N/WzCFfAMbUk=' | openssl enc -d -base64 -rc4 -pass pass:password	使用命令行方式直接使用"password"解密字符串；


算哈希
------------------------------------------------------------------
echo -n 'string' | openssl md5	计算字符串的MD5值
echo 'string' | openssl md5	计算的是带换行符的string的MD5；
openssl md5	窗口输入要被计算的字符串，以CTRL+D结束；
openssl md5 filename	得到filename的MD5值；
