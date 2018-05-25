//1使用ifstream读取到文件尾后，第二次读取（不管是否是同一文件）前一定要使用clear()函数声明流对象中的错误位，
//否则第二次读取始终是false
//2如果某个函数以iostream类型为形参，不要声明为const，因为这个对象可能要在读取过程中对成功与否，符号位等进行设置，
//加const修饰会出错，编译就会出现一些莫名的问题
#include<iostream>
#include<fstream>
using namespace std;
//void find(const ifstream&infile)//编译出错，不能加const，因为读文件在末尾或者类型转换等要对infile对象进行设置
//如果定义为const会出错
void find(ifstream&infile)
{
	char ch;
	if((ch=infile.get())=='z')
		cout<<"Yes"<<endl;
}
int main()
{
	return 0;
}