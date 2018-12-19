#include<iostream>
#include"voip_private_dsph_head.h"
#include"voip_dispatch_protocol.pb.h"
#include<fstream>
using namespace std;
using namespace da::voip::dispatch;
int main(int argc,char* argv[])
{
	if(argc!=2)
	{
		cout<<"argc must be 2"<<endl;
		return -1;
	}
	cout<<"helloworld"<<endl;
	fstream infile(argv[1]);
	if(!infile)
	{
		cout<<"file open error"<<endl;
		return -1;
	}
	char buf[4096];
	int x;
	infile>>hex>>x;
	int i=0;
	while(infile)
	{
		buf[i++]=x;
		infile>>hex>>x;
	}
	cout<<"data_num:"<<i<<endl;
	infile.close();
	
	char*data=buf+68;
	VoipPrivatedDsphHead *phead=(VoipPrivatedDsphHead*)data;
	VoipDispatchHead head;
	VoipDispatchBody body;
	if(!head.ParseFromArray(data+sizeof(VoipPrivatedDsphHead),phead->head_len))
	{
		cout<<"head parse error"<<endl;
		return -1;
	}
	if(!body.ParseFromArray(data+sizeof(VoipPrivatedDsphHead)+phead->head_len,phead->body_len))
	{
		cout<<"body parse error"<<endl;
		return -1;
	}
	cout<<"head_len:\t"<<phead->head_len<<endl;
	cout<<"body_len:\t"<<phead->body_len<<endl;
	head.PrintDebugString();
	body.PrintDebugString();
	
	return 0;
}
