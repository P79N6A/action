
免密码登陆
==============================
1.在linux服务器用ssh-keygen -t rsa生成一个私钥文件A；（这个文件A是一个真正的存储私钥的文件，要妥善保管）
2.把A下载到本地，用putty-gen.exe把Load进来；
3.这时会生成一个public key在上面的滚动窗口里，把这个字符串拷贝到目标机器的~.ssh/authorized_keys里面；点击按钮"Save private key"把它putty格式的私钥文件保存用于用plink.exe来登陆；