int len=msg.ByteSize();	//得到pb把msg打包后的二进制长度；
bool	msg.SerializeToArray(buf,sizeof(buf));	//把信息打包进二进制数组；
bool	msg.SerializeToString(string*);	//把信息打包进std::string串；
inner_struct*is=msg.mutable_inner_struct();	is->set_xxx();或msg.mutable_inner_struct()->set_xxx();	//创建内部结构体并设置值；看源码mutalbe_*内部会new一个对象，重复调用不会重复new；
release_xxx函数看代码会返回当前内部的结构体指针，并清空与msg的关系；
IsInitlized()	判断所有required字段是否被填写；