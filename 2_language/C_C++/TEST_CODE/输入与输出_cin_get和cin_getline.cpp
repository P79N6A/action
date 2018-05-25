//1cin.get()和cin.getline()的使用一定要很标准，不能为空（只有回车符），为空时要调用无参数的
//2cin.get()跳过，超过数组容量时，要用cin.clear()函数清空错误位
//3使用getline()第一个不能是回车，要不读取会有问题，如果它的前面用了cin>>读的不论是字符
//还是数字，都会留下一个回车给它，导致读取错误，这时要使用get()读过回车，再用getline。所
//以养成习惯，在每一轮使用流运算符之后，使用cin.get()读一次空格，避免下一次调用cin.get()
//或cin.getline()时出现错误。
//4使用cin.getline(char*,int,char)到特定的字符停止后，该行这个特定字符后面所有的字符不会被舍弃，第二次读取会继续。cin.get(char*,int,char)同理。
//istream & get(char *, int, char);
//istream & get(char *, int);
//istream & getline(char *, int, char);
//istream & getline(char *, int);
#include<iostream>
using namespace std;
int main(void)
{
	//char str[5];
	//cout<<str;//没有初始化直接输出的是乱码，即没有'\0'，遇到一个随机的'\0'时会停止
	//cout<<endl;
	//cout<<"cin.getline==================================================="<<endl;
	//str[0]='\0';
	//cin.getline(str,5);//如果该行输入字符数小于4可以正常输出，如果字符数大于4则后面所有输入
	////全部被忽略(在不使用cin.clear()的情况下)，如果这里直接回车，则也按两次回车跳过两次读取
	//cout<<str<<endl;
	//cin.clear();//使用cin.clear()可以使getline从后面的字符继续读取
	//cin.getline(str,5);
	//cout<<str<<endl;
	//cout<<"cin.get==================================================="<<endl;
	//cin.clear();
	//cin.get();//如果直接输入的是回车，可以读过
	//cin.get(str,5);//输入字符数小于4正常输出，如果输入字符数大于4，则读4个数，第二次读
	////取时从第5个数继续读，如果输入回车，则后面所有输入被忽略，相当于get()每次都认
	////为自己到了行末，而停留在原地不读取。无参数的cin.get()，可以读过
	////回车，再用cin.get()正常读取下一行
	//cout<<str<<endl;
	//cin.get(str,5);
	//cout<<str<<endl;
	////cin.get(str1).get(str2).get(str3)//因为cin.get()和cin.getline()函数返回的是一个cin object所以可以连续
	////调用
	cout<<"---------------------------------------------------------------"<<endl;
	{
		cout<<"=>以特定字符终止读入"<<endl;
		char a[100];
		cin.getline(a,100,':');cout<<a<<endl;
		
		cin.getline(a,100,',');cout<<a<<endl;
		
	}
	return 0;
}