#include<iostream>
#include<map>
#include<string>
using namespace std;
void show(const map<string,int>&m)
{
	for(map<string,int>::value_type a:m)
		cout<<a.first<<'\t'<<a.second<<endl;
}
int main()
{
	cout<<"///////////////////////////////////////////////////////////"<<endl;
	cout<<"=>Direct Element Access of Maps"<<endl;
	{//只有map重载了[]，而multimap没有重载[]
		map<string,int> m{{"zhang liang",23},make_pair("zhao qing",24),multimap<string,int>::value_type("xiao qiang",24),
			pair<string,int>("she",25)};
		show(m);
		m["wuyitian"]=25;//相当于插入了一个新元素；
	
		cout<<m["zhang liang"]<<endl;
		cout<<m.at("wuyitian")<<endl;
	}
	return 0;
}



