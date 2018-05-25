//1可以直接使用cin输入C类型的字符数组
//2可以使用strcpy对已经定义的数组赋值。
#include<iostream>
#include<cstring>
using namespace std;
int main()
{
	cout<<"------------------------------------------------"<<endl;
	{
		cout<<"=>字符数组的初始化"<<endl;
		char s[100];//编译时分配的数组'};
		strcpy(s,"successfull");//使用strcpy赋值很方便
		cout<<"s:"<<s<<endl;
		char a[100]={'a','b','c','d'};cout<<a<<endl;//编译器会自动在最后加字符'\0'
		char b[]={'e','f'};cout<<b<<endl;//由初始化内容来决定数组的大小


	}
	cout<<"------------------------------------------------"<<endl;
#if 0
	{
		cout<<"=>字符数组的输入"<<endl;
		char a[100];
		cin>>a;
		cin.get(a);
		cin.get(a,100);
		cin.get(a,100,'d');
		cin.getline(a);
		cin.getline(a,100);
		cin.getline(a,100,'d');
		unsigned i=0;while((cin.get(a[i++])!='\n');
		//char*s;//这个s并没有分配用于存储字符串的有效地址，所以为一个乱码地址存入数据会崩溃
		//cin>>s;

		//不能对一个char*p直接始用cin，因为这个p还没有指向实际的存储地址，下面这些均运行崩溃。
		//char*a=new char[100];//动态分配的字符数组
		//cin>>a;
		//cout<<a;
		//delete[]a;
		
		char*ps="happy";
		cout<<ps<<endl;
		//cin>>ps;//运行崩溃，不能为一个临时变量赋值，s所指向的地址相当于一个临时变量区，
		/////这与右值引用必须为const如出一辙，右值不能被赋值。
		//cout<<ps;
	}
#endif
	cout<<"------------------------------------------------"<<endl;
	{
		cout<<"=>字符数组的地址："<<endl;
		char s[100];strcpy(s,"zhangliang");
		cout<<"s的地址："<<(void*)s<<endl;
		//cout<<"s的地址："<<(char*)s<<endl;//使用char*不能输出它的地址，输出的是字符串，但是对于将一个字符数组的地址赋给另一个值，可以直接使用数组名或&shuzu[0]
		cout<<&s[0]<<endl;//输出的仍然是这个字符串
	}
	cout<<"------------------------------------------------"<<endl;
#if 0
	{
		cout<<"=>意外输入情况"<<endl;
		char s1[5]={'\0'};
		cin.get(s1,5);
		if(cin)cout<<s1<<endl;//即使输入数据超过4个,也只读四个字符，第二个参数表示读入几个字符
		cin.get(s1,3);
		if(cin)cout<<s1<<endl;
		//cin.clear();
		//cin>>s1;
		//cout<<s1<<endl;
		//cin.clear();
	}
#endif
	return 0;
}