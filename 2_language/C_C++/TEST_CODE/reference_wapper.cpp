#include<iostream>
#include<vector>
#include<functional>
using namespace std;
class Item
{
public:
	Item(string s,double d):study(s), action(d){}
	string getStudy(){return study;}
	double getAction(){return action;}
private:
	string study;
	double action;
};
int main()
{
cout<<"不使用reference_wrapper<>包装========================================================="<<endl;
	vector<Item> v;
	//vector<Item&>v0;//出错，不能这样定义
	v.push_back(Item("zhangLiang",5));
	v.push_back(decltype(v)::value_type(Item("Liang",7)));
	cout<<v[0].getStudy()<<v.at(0).getAction()<<endl;
cout<<"使用reference_wrapper<>包装============================================================"<<endl;
	std::vector<std::reference_wrapper<Item>> books;
	//books.push_back(Item("cheng",100));//出错，不能这样传递值
	Item it("cheng",100);
	books.push_back(it);
	cout<<books[0].get().getStudy()<<endl;
	return 0;
}