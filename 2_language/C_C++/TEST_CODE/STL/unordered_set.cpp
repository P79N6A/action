#include<iostream>
#include<iomanip>
#include<unordered_set>
#include<algorithm>
#include<iterator>
using namespace std;
int main()
{
	unordered_set<int> s1;
	s1.insert({1,3,210,7});
	copy(s1.cbegin(),s1.cend(),ostream_iterator<int>(cout," "));
	cout<<endl;

	auto pos=s1.cend();
	//cout<<*pos<<endl;//这里运行崩溃，cend()返回的是一个空值 
	//它是最后一个值访问完了之后的一个指针
	int i=0;
	for(pos=s1.cbegin(); pos!=s1.cend(); pos++, i++)//把这个unordered_set中的值依次输出
	{
		cout.setf(ios::left);
		cout<<setw(10)<<i<<*pos<<endl;
	}
	return 0;
}