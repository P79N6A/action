


第一种写法
---------------------------
	IFS= read var << EOF
	$(foo)
	EOF
	
	
第二种写法
---------------------------
echo "hello world" | { read test; echo test=$test; }

第三种
---------------------------
read_from_pipe() { read "$@" <&0; }
