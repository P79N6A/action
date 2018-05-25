#include<iostream>
using namespace std;
template<typename T>
void show(T t,int m,int n)
{
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<n;j++)
		{
			cout<<t[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
}
int main()
{
	cout<<"//////////////////////////////////////////////////////////////////////////"<<endl;
	{
		cout<<"=>静态定义一个二维数组"<<endl;
		int a[2][3];show(a,2,3);
		int b[][2]={1,2,3,4};show(b,2,2);
		int c[][2]={1,2,3};show(c,2,2);
		int d[4][3]={0};show(d,4,3);//所有的都会被初始化为0
	}
	cout<<"//////////////////////////////////////////////////////////////////////////"<<endl;
	{
		cout<<"=>动态分配一个二维数组"<<endl;
		cout<<"第一种，各个行依次分配:"<<endl;
		int m=5;int n=3;
		int **a=new int*[m];
		for(int i=0;i<m;i++)
		{
			a[i]=new int[n];//其实每一行也可以分配不一样的
		}

		for(int i=0;i<m;i++)
			delete[] a[i];
		delete []a;
		cout<<"第二种，一次性分配所有空间"<<endl;
		a=new int*[m];
		a[0]=new int[m*n];
		for(int i=1;i<m;i++)
		{
			a[i]=a[i-1]+n;
		}
		delete []a[0];
		delete []a;
	}
	cout<<"//////////////////////////////////////////////////////////////////////////"<<endl;
	{
		cout<<"=>二维数组的Acess"<<endl;
		int a[2][3];
		for(int i=0;i<2*3;i++)
			a[0][i]=i;//a[i][j]相当于a[0][i*n+j];//只有当二维数组的每一行都是依次排列时才可以这样用，即一次性分配所有空间的情况才可以这样用。这个例子也表明静态分配的数组就是一次性分配所有空间
		show(a,2,3);
	}
}



