echo 'abc' | openssl enc -base64	使用base64算法编码;
echo 'abc' | openssl enc -e -base64 -rc4 	使用rc4算法加密，密码通过命令行提示输入；如果没有-base64则会生成二进制数据；

echo 'abc' | openssl enc -e -base64 -rc4 -pass pass:password	使用命令行方式直接使用"password"加密abc；
echo 'abc' | openssl enc -e -base64 -rc4 -pass file:filename	打开文件filename并取第一行为密码

echo 'U2FsdGVkX1+9eA1N/WzCFfAMbUk=' | openssl enc -d -base64 -rc4 -pass pass:password	使用命令行方式直接使用"password"解密字符串；