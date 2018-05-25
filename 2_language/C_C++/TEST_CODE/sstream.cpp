#include<iostream>
#include<string>
#include<sstream>
using namespace std;
int main()
{
	cout<<"---------------------------------------------------------"<<endl;
	cout<<"=>istringstream"<<endl;
	istringstream iss(string("iss: 3.14 spring"));
	istringstream temp("sky");//这样也可以
	//iss=string("iss: 3.14 spring");//不能使用这种方式赋值，可能iss没有重载"="
	string s;
	while(iss>>s) cout<<s<<" ";cout<<endl;
	cout<<iss.str()<<endl;//使用">>"后iss的值并没有改变，可能重载的'>>'内部维持着一个记录
	cout<<"---------------------------------------------------------"<<endl;
	cout<<"=>ostringstream"<<endl;
	ostringstream oss("man");//这个"man"不起作用
	oss<<"future tree river grass";
	s=oss.str();cout<<s<<endl;
	cout<<"---------------------------------------------------------"<<endl;
	cout<<"=>sstringstream"<<endl;
	stringstream ss("begin:");//这个"begin"不能起到预先放一个"begin"的作用
	ss<<"a b c d e f";
	int n=3;
	while(n--){ss>>s;cout<<s<<',';}cout<<endl;
	ss<<" insert";cout<<ss.str()<<endl;
	while(ss>>s)cout<<s<<",";cout<<endl;//它会从前面没有输出完的继续输出

	ss>>s;cout<<s<<endl;//会连续输出两次最后一个
	ss>>s;cout<<s<<endl;

	ss.clear();cout<<ss.str()<<endl;//这个clear()只是清空一些错误位，不会清空所有数据。

	ss.flush();cout<<ss.str()<<endl;//flush同样不会清空

	ss<<" A ";cout<<ss.str()<<endl;
	ss.seekg(ss.beg);ss>>s;cout<<s<<endl;//ss就相当于一个文件一样，可以将指针移到文件头继续读
	
	ss.str("");cout<<ss.str()<<endl;//对stringstream进制清空


	return 0;
}