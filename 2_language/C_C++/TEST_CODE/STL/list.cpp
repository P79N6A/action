#include<iostream>
#include<list>
using namespace std;
int main()
{
	auto show=[](const list<int>& l)
	{
		if(l.empty()){cout<<"the list is empty"<<endl;return;}
		for(auto i:l)
		{
			cout<<i<<' ';
		}
		cout<<endl;
	};
	cout<<"////////////////////////////////////////////////////////////////"<<endl;
	cout<<"=>Insert and Remove Operations of Lists:"<<endl;
	{//基本操作与vector都是一样的；
		list<int> l{1,2,3,4,5};show(l);
		l.remove(4);show(l);
		l.remove_if([](const int i){return i>=3;});show(l);//移除所有大于3的数；
	}
	cout<<"////////////////////////////////////////////////////////////////"<<endl;
	cout<<"=>Special Modifying Operations for Lists:"<<endl;
	{
		list<int> l{1,2,3,4,5};
		l.push_back(5);l.push_back(5);show(l);
		l.unique();show(l);//移除所有重复的数；
		cout<<endl;
		l.push_front(1);l.push_back(4);show(l);
		l.unique([](int i,int j){return i==j;});show(l);//当这个链表中存储的是类对象时，这时如何判断相等或重复就很重要
		cout<<"----------------------------"<<endl;
		list<int> l1{1,2,3};
		list<int>::iterator p=l.begin();
		//p++;p++;
		l.splice(l.begin(),l1);show(l);show(l1);//把l1全部移动到l.begin()的前面，被移对象相应位置会为空
		cout<<endl;
		l.assign({1,2,3,4,5});l1.assign({1,2,3});show(l);show(l1);
		p=l1.end();p--;
		l.splice(l.begin(),l1,p);show(l);show(l1);//把l1的最后一个元素移到l的最前面
        cout<<endl;
		l.splice(l.begin(),l1,l1.begin(),l1.end());show(l);show(l1);//把l1所有元素移到l的前面
		cout<<"----------------------------"<<endl;
		l.sort();show(l);//升序排列
		l.sort([](int i,int j){return i>j;});show(l);//降序排列
		l.reverse();show(l);
		l.merge(list<int>{3,3,4});show(l);

	}
	return 0;
}





