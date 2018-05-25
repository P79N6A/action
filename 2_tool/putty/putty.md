
Putty
====================================
<https://www.chiark.greenend.org.uk/~sgtatham/putty/>


putty使用配置
====================================
putty配置窗口中每一项配置都是针对一个特定的session而言，所以在配置前要load已经保存的session，并在配置完以后save。

在Connection栏中，设置发送空包，以长时间无操作中断。

在Window-Translation中，设置连接所用的字符集，putty与目标机器同时设置utf8可以完美实现中文编辑远程文件，但secureCRT则还是乱码。

在Connection-SSH-Auth中，设置私钥路径可以实现免输密码登陆，前提是要提前生成密钥对并在服务器提前设置公钥。


文件传输
====================================
Putty自带的pscp因为有选项可以直接指定密码或私钥文件，所以下载文件是十分方便的。


特别问题
====================================

putty遇到ctrl+s
---------------------------
问题描述：用putty玩linux的时候由于以前用window 习惯写完东西按一下ctrl+s 保存，但是在putty一按下就不能再输入了。
后来查找到：ctrl+s 是putty的一个命令大概是这样子：”Long long ago“ 某些旧的”哑终端“会在发送过来的数据太多，
显示速度跟不上时发送一个Ctrl+s让对方等一下，然后再准备好继续显示时发送一个Ctrl+q。
Putty“兼容”了这个特性。也有人说是Ctrl+s是锁定屏幕用的 。
不管是怎样，当你不小心在Putty中按了Ctrl+s后再按一下Ctrl+q就OK了。