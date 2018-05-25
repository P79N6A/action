//1当用ofstream对象读到文件结尾时，操作系统通常会以一个特殊的数字不属于ASCII码，放在文件最后一个当作文件结束标志，所以ostream读完属于文件的最后一个字符，它并不会认为到达了文件尾，而是要尝试那个文件结束符，读结束符失败后ofstream对象的bool值会是一个false(ofstream.get()返回的值是-1)，表示读取失败，且判断如果这个符号就是文件结束符，那么eof()就会返回一个true；于此，在处理文件结束时要分外注意，如下面的测试代码。
#include<iostream>
#include<fstream>
using namespace std;
int main()
{
	ofstream outfile;
	outfile.open("test.txt");
	if(!outfile.is_open()){cerr<<"outfile error"<<endl;exit(1);}
	outfile<<"I will be a rich man!"<<endl;
	outfile.close();
	cout<<endl<<"--------------------------------------------------------"<<endl;
	ifstream infile;

	infile.open("test.txt");
	if(!infile.is_open()){cerr<<"infile error"<<endl;exit(1);}
	cout<<char(-1)<<"|"<<endl;
	cout<<char(-1-8)<<"|"<<endl;
	cout<<endl<<"--------------------------------------------------------"<<endl;
	cout<<"=>第一种方式："<<endl;
	while(!infile.eof())
	{
		cout<<char(infile.get());//对于文件结束符,infile.get(ch)读取失败，返回一个int值-1,当然bool值为false，这个char(-1)是一个类似空格的看不见的字符。于是这个莫名字符被输出。所以除正常文件包含的内容又多出来一块就是这个莫名字符。所以while内部输出时，要用if来判断一下得到的值：如下：
		//int ch;ch=infile.get();if(ch>=0) cout<<char(ch);
	}
	cout<<"|"<<endl;
	if(infile.eof())
		infile.clear();
	else 
	{cerr<<"unknow error"<<endl;exit(1);}
	cout<<endl<<"--------------------------------------------------------"<<endl;
	cout<<"=>第二种方式："<<endl;
	infile.seekg(ios::beg);
	char ch;
	while(!infile.eof())
	{
		infile.get(ch);//与第一种方式相同的问题，都是读取了文件结束符再输出
		cout<<ch;
	}
	cout<<"|"<<endl;
	if(infile.eof())
		infile.clear();
	else 
	{cerr<<"unknow error"<<endl;exit(1);}
	cout<<endl<<"--------------------------------------------------------"<<endl;
	cout<<"=>第三种方式："<<endl;
	infile.seekg(ios::beg);
	
	while(infile.get(ch))//直接判断了有效性，正确
		cout<<ch;
	cout<<"|"<<endl;
	return 0;
}