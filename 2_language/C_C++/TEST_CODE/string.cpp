//1string重载了很多运算符，可以直接使用
//2string内部打包的是一个C风格的字符数组a，string[i]相当于a[i],&string[i]就是&a[i]和a+i;
#include<iostream>
#include<string>
#include<cstdlib>
#include<fstream>
using namespace std;
void strcount(const string& str);//该行编译会出错：不识别string，如果把using namespace std;移到全局
//或在前面加using std::string就没有错了
int main(void)
{
	cout<<"---------------------------------------------------------------------"<<endl;
	{
		cout<<"=>string初始化方式"<<endl;
		char a[50]={"strive to be a sucessfull man"};
		//scanf("%s",a);//C语言的输入风格
		//cin>>a;//cin的输入风格
		////cin.get(b,20)或cin.getline(b,20);//也可以这样用
		string b(a);cout<<b<<endl;//可以直接用C风格的字符数组初始化一个string
		string c(a,5);cout<<c<<endl;//用a的前5个字符初始化string
		string d(a,70);cout<<d<<endl;//用从a地址开始后的70个字符初始化string，可以越过'\0'和数组界线
		string e(a,4,8);cout<<e<<endl;//从第4个开始取8个字符初始化string
		string f(a+4,a+8);cout<<f<<endl;//取第4个到第8个字符初始化string
		string g(&a[4],&a[8]);cout<<g<<endl;
		cout<<endl;
		string h(a+4,a+80);cout<<h<<'|'<<endl;//从第4个数开始读80个字符，可以越过'\0'和数组界线
		string i(10,'o');cout<<i<<endl;//用10个'o'初始化
		string j("I am zhangliang");cout<<j<<endl;
		string jj;//空字符串
		string k(j,5);cout<<k<<endl;//从第从字符串j的第6个开始（下标5）到尾初始化string，第二个参数默认npos
		string l(j,5,6);cout<<l<<endl;//从下标为5开始的字符开始读6个字符
		string m(j,0,30);cout<<m<<'|'<<endl;//当第二个数大于字符串长度时，读到尾就停止
		string n(&j[5],&j[8]);cout<<n<<endl;//从第5个下标读到第8-1个坐标
		string ll(&j[5],6);cout<<ll<<endl;//同l的初始化。
		cout<<endl;
		string o(&j[5]);cout<<o<<endl;//从第5个下标开始到尾初始化string，第二个参数默认npos

		cout<<"-----------------------"<<endl;
		string sa;
		//getline(cin,sa);cout<<"="<<sa<<endl;//将一行字符串读入sa，不可以使用cin.getline()，因为该函数在string类之前出
		////现，它不接受string object，见资料《C++ Primer Plus》P138
		//getline(cin,sa,':');cout<<"="<<sa<<endl;//读入一行字符，遇到':'时停止,':'也不属于字符串，但会输出流中提取出来，就像'\n'一样。
		//cout<<"------------------------"<<endl;
		//while(cin.get()!='\n');
		//getline(cin,sa,'\n');cout<<"="<<sa<<endl;//即使显式指定截止字符串，回车符也要被读入，但不属于字符串
		//get(sa,':');//针对string没有get函数。没有这个必要，因为有getline就够了。
		
		//cout<<"------------------------------------------------------------------------------"<<endl;
		//string s[4];
		//for(int i=0;i<4;i++)
		//{
		//	getline(cin,s[i],',');//以逗号为分隔符读取字符，逗号不是字符串的一部分
		//	cout<<s[i]<<" "<<endl;
		//}
	}
	cout<<"---------------------------------------------------------------------"<<endl;
	{
		string a="zhang";
		if(a[0]=='z')//string[i]可以和一个char类型比较
			cout<<"string[i]==char"<<endl;
		a[2]='s';//可以直接改变其中某个字符的值
		cout<<a<<endl;
	}
	cout<<"---------------------------------------------------------------------"<<endl;
	{
		cout<<"string不能与一个其它内置非字符或字符串类型相加构成新string"<<endl
			<<"除非重载这个+运算符"<<">>>>>>>>>>>>>>>>>"<<endl;
		string a("jian");
		//cout<<(a+100)<<endl;
		char b[100];
		cout<<a+string(_gcvt(3.141592653,5,b))<<endl;
		cout<<b<<endl;
	}
	cout<<"---------------------------------------------------------------------"<<endl;
	{
		cout<<"string.length()\string.size()是一样的（版本问题导致）"<<">>>>>>>>>>>>>>>>>"<<endl;
		//string::npos是一个string对象定义时所能包含的最大字符数
		cout<<"string::npos:"<<string::npos<<endl;
		string a("think and move");
		cout<<"a.length():"<<a.length()<<endl
			<<"a.size():"<<a.size()<<endl;
	}
	cout<<"---------------------------------------------------------------------"<<endl;
	{
		cout<<"ifsream.open()或ofstream.open()要求C风格的输入，不能是string"<<">>>>>>>>>>>>>>>>>"<<endl;
		cout<<"使用file.open(str.c_str);或file.open(str.data())进行转化";
	}

	return 0;
}