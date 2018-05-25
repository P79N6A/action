//编写一个在当前文件下寻找包含特定关键字的文件
#include<iostream>
#include<fstream>
#include<string>
#include<io.h>
#include<locale>
using namespace std;
class File
{
public:
	File(wstring na=L"no file name", int co=0){name=na;wordCount=co;}
	wstring name;//记录包含所查字符的文件有哪些
	int wordCount;//记录某个文件中含有多少个所查字符
};
class FileList
{
public:
	FileList();
	~FileList();
public:
	void push(const File &fi);
	void sort();
	void show();
	void reSet();
private:
	File*list;
	int length;
	int capacity;
};
//class Word
//{
int findWord(ifstream&infile,const string&word);
int main()
{
	_wfinddata_t c_file;
	intptr_t hFile;
	ifstream infile;
	FileList nameList,errorList;
	wstring mulu;
	string word;
	char ch;
again:	{
		locale loc("chs");//设置语言环境后，使用gcc编译器会出错，使用vs编译器不会出错
		wcout.imbue(loc);
		//mulu=L"E:\\A_C++\\TEST_CODE\\";//注释它生成的程序exe可以对当前文件下进行寻找，而不是始终是一个程序
		
		nameList.reSet();errorList.reSet();
		if((hFile=_wfindfirst((mulu+L"*.cpp").data(),&c_file))==-1)
		{cout<<"no such files"<<endl;exit(1);}
		cout<<"Please enter the word to find:";
		getline(cin,word);//使用geline可以读入空格
		cout<<"要查的字符为："<<word<<endl;
	}
	do{
		locale loc("");
		//wcout<<c_file.name<<endl;
		infile.open((mulu+c_file.name).data(),ios::in);
		if(!infile){
			wcout<<"can't open the file :"<<(mulu+c_file.name).data()<<endl;
			errorList.push(File(c_file.name));
			infile.clear();continue;}
		else{
			//nameList.push(File(c_file.name));
			nameList.push(File(c_file.name,findWord(infile,word)));
			infile.close();infile.clear();//必须clear，否则同一个对象第二次读取要出问题
		}
	}while(_wfindnext( hFile, &c_file ) == 0);
	_findclose(hFile);
	{
		cout<<endl<<"打不开的文件："<<endl;
		errorList.show();
		cout<<endl<<"可以正常打开的文件："<<endl;
		nameList.sort();
		nameList.show();
	}
	cout<<"Search the word again?(y/n):";
	while(!((ch=cin.get())=='y' || ch=='n'));
	if(ch=='y')
	{
		while(cin.get()!='\n');cin.clear();
		goto again;
	}

	return 0;
}
FileList::FileList()
{
	capacity=10;
	length=0;
	list=new File[capacity];
}
FileList::~FileList(){delete[]list;}
void FileList::push(const File&fi)
{
	if(length>=capacity)
	{
		capacity+=10;
		File*p=new File[capacity];
		for(int i=0;i<length;i++)
		{
			p[i]=list[i];
		}
		delete []list;list=p;p=0;
	}
	list[length]=fi;length++;
}
void FileList::sort()
{
	for(int i=0;i<length-1;i++)
		for(int j=i;j<length;j++)
		{
			File temp;
			if(list[i].wordCount<list[j].wordCount)
			{
				temp=list[i];
				list[i]=list[j];
				list[j]=temp;
			}
		}
}
void FileList::show()
{
	if(length==0)
	{
		wcout<<"NO such file"<<endl;
		return ;
	}
	for(int i=0;i<length;i++)
	{
		if(list[i].wordCount>=1)//存在多于一个关键字的才输出
			wcout<<L"["<<list[i].wordCount<<L"]"<<list[i].name<<endl;
	}
}
void FileList::reSet()
{
	delete []list;
	capacity=10;
	length=0;
	list=new File[capacity];
}
int findWord(ifstream&infile,const string&word)
{
	char ch;int count=0;
	while(!infile.eof())
	{
		if((ch=infile.get())==word[0])
		{
			for(int j=1;j<word.length();j++)
			{
				if((ch=infile.get())==word[j])
				{
					if(j==word.length()-1)
						count++;
					continue;
				}
				else 
				{
					if(infile.eof())
						return count;
					break;
				}
			}
		}
	}
	return count;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// crt_find.c
// This program uses the 32-bit _find functions to print
// a list of all files (and their attributes) with a .C extension
// in the current directory.
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <io.h>
//#include <time.h>
//
//int main( void )
//{
//   struct _finddata_t c_file;
//   intptr_t hFile;
//
//   // Find first .c file in current directory 
//   if( (hFile = _findfirst( "E:\\A_C++\\TEST_CODE\\*.cpp", &c_file )) == -1L )
//      printf( "No *.c files in current directory!\n" );
//   else
//   {
//      printf( "Listing of .c files\n\n" );
//      printf( "RDO HID SYS ARC  FILE         DATE %25c SIZE\n", ' ' );
//      printf( "--- --- --- ---  ----         ---- %25c ----\n", ' ' );
//      do {
//         char buffer[30];
//         printf( ( c_file.attrib & _A_RDONLY ) ? " Y  " : " N  " );
//         printf( ( c_file.attrib & _A_SYSTEM ) ? " Y  " : " N  " );
//         printf( ( c_file.attrib & _A_HIDDEN ) ? " Y  " : " N  " );
//         printf( ( c_file.attrib & _A_ARCH )   ? " Y  " : " N  " );
//         ctime_s( buffer, _countof(buffer), &c_file.time_write );
//         printf( " %-12s %.24s  %9ld\n",
//            c_file.name, buffer, c_file.size );
//      } while( _findnext( hFile, &c_file ) == 0 );
//      _findclose( hFile );
//   }
//}