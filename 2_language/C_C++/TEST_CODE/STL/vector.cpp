#include<iostream>
#include<vector>
#include<algorithm>
#include<iterator>
using namespace std;
int main()
{
	auto lambdaShow=[](const int &i){cout<<i<<" ";};
	cout<<"////////////////////////////////////////////////////"<<endl;
	cout<<"=>the constructors and destructor of vector:"<<endl;
	{

		vector<int> v1={1,2,3,4,5};
		vector<int> v2(10);//会自动将这10个元素初始化为0
		vector<int> v3(10,1);//创建10个元素的向量并初始化为1；
		vector<int> v4(v3.begin(),v3.end());//用v3初始化v4
		for_each(v4.cbegin(), v4.cend(), lambdaShow);cout<<endl;//用Lambda表达式输出
		copy (v1.cbegin(), v1.cend(), ostream_iterator<int>(cout," "));cout<<endl;//用迭代器输出；
	}
	cout<<"////////////////////////////////////////////////////"<<endl;
	cout<<"=>Nonmodifying Operations of Vectors:"<<endl;
	{
		vector<int> v{1,2,3,4,5};
		cout<<"empty?:"<<v.empty()<<endl;
		cout<<v.size()<<endl;
		cout<<v.max_size()<<endl;
		cout<<v.capacity()<<endl;
		v.shrink_to_fit();//减小capacity以适应较短的向量；
		//v1.reserve(80);//reserve改变的是capacity
	////说明：size()输出的是向量长度，max_size()是内存中可供分配的最大值
	////capacity是当前为v1预留的可以分配内存元素的数量（相当于是为它预留的）
		cout<<"-----------------------------------------------"<<endl;
		vector<int> v1{1,2,3};
		cout<<"v1>v?:"<<(v1>v)<<'\t'<<"v>v1?:"<<(v>v1)<<endl;
	}
	cout<<"////////////////////////////////////////////////////"<<endl;
	cout<<"=>Assignment Operations of Vectors:"<<endl;
	{
		vector<int> v{1,2,3,4,5};
		for(int a:v)cout<<a<<' ';cout<<endl;
		vector<int> v1;
		v1.assign(v.begin(),--v.end());//相当于在原地址上重新建了一个同名向量
		for(auto a:v1)cout<<a<<' ';cout<<endl;
		v1.assign({11,12,13});
		for(auto a:v1)cout<<a<<' ';cout<<endl;
		v.assign(3,10);
		for(auto a:v)cout<<a<<' ';cout<<endl;
		v.swap(v1);//交换两个变量的数据:(交换的其实只是动态分配的指针地址，并更新其它信息，这样更快）
	}
	cout<<"////////////////////////////////////////////////////"<<endl;
	cout<<"=>Direct Element Access of Vectors:"<<endl;
	{
		vector<int> v{1,2,3,4,5};
		cout<<v[1]<<'\t'<<v.at(1)<<'\t'<<v.front()<<'\t'<<v.back()<<endl;
	}
	cout<<"////////////////////////////////////////////////////"<<endl;
	cout<<"=>Insert and Remove Operations of Vectors:"<<endl;
	{
		auto show=[](const vector<int>&v)
		{
			if(v.empty()){cout<<"the vector is empty"<<endl;return;}
			for(auto a:v)
				cout<<a<<' ';
			cout<<endl;
		};
		vector<int> v{1,2,3,4,5};
		v.push_back(6);
		v.pop_back();
		v.insert(v.begin(),10);show(v);
		v.insert(++v.begin(),2,11);show(v);
		vector<int> v1{101,102,103};
		v.insert(v.end(),v1.begin(),v1.end());show(v);//v.end()这个指针处不存放任何元素，仅仅是作为一个结束指针；
		v.emplace(v.begin(),-1);show(v);
		v.emplace_back(201);show(v);
		v.erase(v.end()-1);show(v);//不能删除v.end()这个最后无元素的点
		v.erase(v.end()-3,v.end());show(v);//v.end()处的值并不会被删除
		v.resize(10);show(v);//默认添0
		v.resize(15,301);show(v);//以第二个参数填充扩充的向量；
		v.clear();show(v);
	}
	cout<<"////////////////////////////////////////////////////"<<endl;
	cout<<"=>vector<bool>:"<<endl;
	{//布尔类型是以位存储的，更省空间
		auto show=[](const vector<bool>&v)
		{
			if(v.empty()){cout<<"the vector is empty"<<endl;return;}
			for(auto a:v)
				cout<<a<<' ';
			cout<<endl;
		};
		vector<bool> v(4);show(v);//全部以0初始化；
		v.flip();show(v);//全部取反
		v[3]=false;show(v);
		v[2]=v[3];show(v);
	}

	return 0;
}



