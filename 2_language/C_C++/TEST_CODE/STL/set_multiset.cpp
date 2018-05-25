#include<iostream>
#include<set>
#include<algorithm>
#include<iterator>
using namespace std;
template<typename T>
void show(const T&s)
{
	copy(s.begin(), s.end(), ostream_iterator<int>(cout," "));//用迭代器输出
	cout<<endl<<endl;
}
int main()
{
	cout<<"////////////////////////////////////////////////////////////////"<<endl;
	cout<<"=>Insert and Remove Operations of Sets and Multisets:"<<endl;
	{//基本操作与vector都是一样的；
		set<int> s;
		s.insert(1);//在s.end()的前面插入
		s.insert({2,4,5,6});
		show(s);
	}
	cout<<"////////////////////////////////////////////////////////////////"<<endl;
	cout<<"=>Special Search Operations of Sets and Multisets:"<<endl;
	{
		multiset<int> s;
		s.insert({1,2,3,3,4,5,5});
		cout<<s.count(5)<<endl;
		cout<<*s.find(4)<<endl;
		cout<<"lower_bound(3):"<<*s.lower_bound(3)<<endl;
		cout<<"upper_bound(3):"<<*s.upper_bound(3)<<endl;
		show(s);
		auto range=s.equal_range(3);//这个range的第一个指针和第二个指针其实就是lower_bound和upper_bound
		cout<<*range.first<<'\t'<<*range.second<<endl;
		s.insert(s.upper_bound(3),3);//它是在upper_bound的前面插入
		show(s);
	}
	return 0;
}