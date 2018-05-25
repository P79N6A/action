//1cin读取不成功时（可能由于到文件尾，可能类型不匹配，可能读完\n没有可读入的内容等）,cin可以被赋于
//一个bool类型形参，它会被转化为一个bool类型，表示上次读入是否成功。如if(cin)。
//2如果上一次cin读入错误（类型不匹配或遇文件尾等）后不清除错误位，
//后面cin所有读入都将不起作用，包括cin.get()
//3如果输入输出的数据是十六进制，则必须使用cin>>hex,cout<<hex；十进制是cin>>dec;cout<<dec;
//如果不进行设定，对于0x1a的输入，得到的会是0，并用cin的布尔类型为真。
//原因是数据会被认为是正是常读入，遇到字符x停止，如果不读过中间非法字符,后面
//所有的输入都会不执行。因为对于正常的非字符数据输入：32dfff,其中32会被正确读入
//而后面字符cin会尝试去读但由于类型不匹配不成功在第二次造成failbit。
//如果输入流是3847,?前面数字后面是非字母字符，第一次cin输入就是乱码
//4很有用的几个函数：
//cin>>;cin.read;cin.get();cin.getline;
//cin.gcount();cin.ignore();
//cin.pushback();cin.peek();cin.unget;
#include<iostream>
#include<sstream>
#include<string>
using namespace std;
int main()
{
#if 0
	{
		char ch;
		while(cin>>ch)//这种方式读取会忽略空格tab和回车
		{
			cout<<ch;
			//if(ch=='\n')break;//无效，会忽略回车
			if(ch=='O')break;
		}
		cin.get();//读掉回车符
		cout<<endl<<"----------------------------------------------------"<<endl;
		while(cin.get(ch))//这种方式会读取所有的字符，返回的是一个cin对象，会被自动转化为一个
			//bool类型值，如果读取失败返回的是false，参见《C++ Primer Plus》第四章
		{
			cout<<ch;
			if(ch=='\n')break;
		}
		cout<<endl<<"----------------------------------------------------"<<endl;
		while(ch=cin.get())//这种方式会读取所有的字符，返回的是一个int类型(当然与对应的ASCII字符值相等)
		{
			cout<<ch;
			if(ch=='\n')break;
		}
	}
#endif
#if 0
	cout<<endl<<"----------------------------------------------------"<<endl;
	{
		int temp;
		while(!(cin>>temp))//如果输入的不是int类型，而是其它字符，就会返回一个false cin，第二次读
		{//入必须对cin使用clear()函数，且要跳过非法字符，才可以正确读入
			//如果输入的是123abc那么会把前面的123正确读入，后面字符舍去
			cin.clear();
			cout<<"error input , enter again"<<endl;
			while(cin.get()!='\n');//不断调用cin.get()跃过非法字符
		}
		cout<<temp<<endl;
		unsigned int a;
		if(cin>>a)cout<<"a:"<<a<<endl;//对于一个unsigned int类型输入一个负数，cin返回的是一个true，但数据对不上号
		else cout<<cin<<endl;
	}
#endif
#if 0
	{
		if(cin.get())cout<<"cin.get()可以读入回车，返回true"<<endl;
	}
	{
		int a,b;
		cin>>a;
		cout<<a<<endl;
		//if(cin.fail()){cout<<"faile bit and clear"<<endl;cin.clear();}
		while(cin.get()!='\n');//如果上一次cin读入错误后不清除错误位，后面cin所有读入都将不起作用，包括cin.get()
		cin>>b;
		cout<<b<<endl;
	}
#endif
#if 0
	cout<<"-------------------------------------------------------------"<<endl;
	{
		int a;//不能正常读入一个十六进行，cin返回的仍是true
		cin>>a;
		if(cin){cout<<"输入的是16进制"<<a<<endl;}
		while(cin.get()!='\n');cin.clear();
		
		cin>>hex;cout<<hex;
		cin>>a;cout<<a<<endl;
	}
#endif
#if 1
	cout<<"---------------------------------------------------------"<<endl;
	{
		cout<<"=>cin读取以逗号为分隔的数据"<<endl;
		int a;
		stringstream ss;
		string s;
		int ch;
		while((ch=cin.get())!='\n')
		{
			ss.clear();
			if(ch==',')
			{
				ss<<s;
				ss>>a;
				cout<<a<<" ";
				s.clear();
			}
			else
			{
				s.push_back(ch);
			}
		}
		ss<<s;ss>>a;cout<<a<<" "<<endl;

	}

#endif
	//cout<<"---------------------------------------------------------"<<endl;
	//{
	//	cout<<"=>跳过一些字符到某个特定的字符"<<endl;
	//	//dkgjsgk:dkjgsdkf输出入它，':'前面的可以被while跳过，后面的被ignore跳过
	//	while(cin.get()!=':');//第一种方式
	//	cin.ignore(100,'\n');//第二种方式
	//	cin.ignore(100,'\n');//第二次使用ignore，如果输入流中没有回车，会等待一个输入跳过到'\n'后。所以使用它们之前一定要确定输入流中有这个字符
	//}

	cout<<"---------------------------------------------------------"<<endl;
	{
		cout<<"=>利用stringstream实现string与double 和int之间的转化"<<endl;
		
		stringstream str;
		string s;
		int a=100;
		str<<a;str>>s;//将一个int型转化为一个字符串
		cout<<s<<endl;
		str.clear();//必须要有它，后面才能正常
	    string pi("3.14");double p;
		str<<pi;//将一个字符串转化为一个double
		str>>p;
		cout<<p<<endl;
	}
	return 0;

}