
#include<stdlib.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include "FtpDownloadFile.h"
#include "pp_socket.h"
namespace fdl {

DownloadFile::DownloadFile() {
	// TODO Auto-generated constructor stub
	cmd_fd=0;
	data_fd=0;
}

DownloadFile::~DownloadFile() {
	// TODO Auto-generated destructor stub
	if(cmd_fd)
	{
		close(cmd_fd);
		cmd_fd=0;
	}
	if(data_fd)
	{
		close(data_fd);
		data_fd=0;
	}
	outfile.close();
}

bool	DownloadFile::init(const char*user,const char*password,const char*url)
{
	strncpy(ftp_user,user,sizeof(ftp_user));
	strncpy(ftp_pass,password,sizeof(ftp_pass));
	strncpy(ftp_addr.str_addr,url,sizeof(ftp_addr.str_addr));
	pp::UDPSocket	us;
	if(!us.set_peer_addr(ftp_addr.str_addr))
	{
		LOG_ERROR("us set peer addr error:%s",ftp_addr.str_addr);
		return false;
	}
	ftp_addr.ip=us._peer_addr.sin_addr.s_addr;
	ftp_addr.port=us._peer_addr.sin_port;

	if(!create_socket(&cmd_fd) || !create_socket(&data_fd))
	{
		LOG_ERROR("create socket error");
		return false;
	}
	return true;
}
bool	DownloadFile::download(const char*file,char(&out_file)[256])
{
//	while(true);//debug dead-loop case test

	char local_full_name[512];
	{
		int i=0;
		for(;CONFIG.stFTP.szLocal_file_path[i];i++)
		{
			local_full_name[i]=CONFIG.stFTP.szLocal_file_path[i];
		}
		local_full_name[i]='/';
		if(!file[0])
		{
			LOG_ERROR("the file is empty");
			return false;
		}
		int j=0;
		while(file[j++]);
		j=j-2;
		while(j>=0)
		{
			if(file[j]=='/')
			{
				break;
			}
			j--;
		}
		i=i+1;
		j=j+1;
		while(file[j])
		{
			local_full_name[i++]=file[j++];
		}
		local_full_name[i]=file[j];
	}
	LOG_INFO("local file path is:%s",local_full_name);

	DIR*dir=opendir(CONFIG.stFTP.szLocal_file_path);
	if(!dir)
	{
		LOG_ERROR("open dir:%s error, create it",CONFIG.stFTP.szLocal_file_path);
		if(-1==mkdir(CONFIG.stFTP.szLocal_file_path,S_IRWXU | S_IRWXG | S_IRWXO))
		{
			LOG_ERROR("mkdir error[%d]%s",errno,strerror(errno));
			return false;
		}
	}
	else
	{
		closedir(dir);
	}
//	if(outfile.open(local_full_name))
//	{
//		LOG_INFO("file:%s alread exist",local_full_name);
//		outfile.close();
//		return true;
//	}

	struct sockaddr_in _peer_addr;
	_peer_addr.sin_family=AF_INET;
	_peer_addr.sin_addr.s_addr=ftp_addr.ip;
	_peer_addr.sin_port=ftp_addr.port;
	if(-1==::connect(cmd_fd,(struct sockaddr*)&_peer_addr,sizeof(struct sockaddr)))
	{
		LOG_ERROR("connect error[%d]%s,%s",errno,strerror(errno),ftp_addr.str_addr);
		return false;
	}

#define A(x)	if(!(x)){LOG_ERROR(""#x":%s",str.c_str());return false;}
	char buf[128];
	std::string str;

	A(expect("220",str))
	snprintf(buf,sizeof(buf),"USER %s\r\n",ftp_user);
	A(command(buf))
	A(expect("331",str))
	snprintf(buf,sizeof(buf),"PASS %s\r\n",ftp_pass);
	A(command(buf))
	A(expect("230",str))
	A(command("PASV\r\n"))
	A(expect("227",str))
	LOG_INFO("pasv give addr:%s",str.c_str());

	char ip[128];
	unsigned short port;
	{
		int i=0;
		while(str[i]!='(')
		{
			if(!str[i])
			{
				LOG_ERROR("the result has no valid addr");
				return false;
			}
			i++;
		}
		i=i+1;//from i index will be ip


		int j=0;
		int n=0;
		for(;i<str.size();i++,j++)
		{
			if(str[i]==',')
			{
				n++;
				if(n==4)
				{
					ip[j]='\0';
					break;
				}
				else
				{
					ip[j]='.';
				}
			}
			else
			{
				ip[j]=str[i];
			}
		}
		int a=0,b=0;
		i++;
		if(i>=str.size()) return false;
		a=atoi(&str[i]);
		i++;
		while(str[i])
		{
			if(str[i-1]==',')
			{
				b=atoi(&str[i]);
				break;
			}
			i++;
		}
		port=(a<<8)+b;
	}
	LOG_INFO("pase ftp give addr:%s:%hu",ip,port);


	snprintf(buf,sizeof(buf),"RETR %s\r\n",file);
	A(command(buf))

	struct sockaddr_in _peer_addr2;
	_peer_addr2.sin_family=AF_INET;
	_peer_addr2.sin_addr.s_addr=inet_addr(ip);
	_peer_addr2.sin_port=htons(port);
	if(-1==::connect(data_fd,(struct sockaddr*)&_peer_addr2,sizeof(struct sockaddr)))
	{
		LOG_ERROR("connect error[%d]%s,%s",errno,strerror(errno),ftp_addr.str_addr);
		return false;
	}
	A(expect("150",str))


	outfile.open(local_full_name,std::ios::out|std::ios::binary);
	if(!outfile)
	{
		LOG_ERROR("open file:%s error",local_full_name);
		outfile.close();
		return false;
	}
	while(true)
	{
		char buf[10240];
		int s=recv(data_fd,buf,sizeof(buf),0);
		if(s==-1)
		{
			if(errno!=EAGAIN && errno!=EWOULDBLOCK && errno!=EINTR)
			{
				LOG_ERROR("recv error[%d]%s",errno,strerror(errno));
				break;
			}
			continue;
		}
		else if(s==0)
		{
			LOG_INFO("connection closed by ftp");
			break;
		}
		else
		{
			outfile.write(buf,s);
		}
	}
	outfile.close();
	A(expect("226",str))

#undef A
	strncpy(out_file,local_full_name,sizeof(out_file));
	LOG_INFO("download file successfully");
	return true;
}
bool		DownloadFile::create_socket(int*fd)
{
	*fd=socket(AF_INET,SOCK_STREAM,0);
	if(*fd==-1)
	{
		LOG_ERROR("socket create error[%d]%s",errno,strerror(errno));
		return false;
	}
	int optval=1;
	if(-1==setsockopt(*fd,SOL_SOCKET,SO_REUSEADDR,(const void*)&optval,sizeof(optval)))
	{
		LOG_ERROR("setsockopt error[%d]%s",errno,strerror(errno));
		return false;
	}
	return true;
}
bool	DownloadFile::command(const char*buf)
{
	LOG_INFO("ftp command:%s",buf);
	if(::send(cmd_fd,buf,strlen(buf),MSG_NOSIGNAL)==-1)
	{
		LOG_ERROR("send error[%d]%s",errno,strerror(errno));
		close(cmd_fd);
		cmd_fd=0;
		return false;
//		if(errno!=EAGAIN && errno!=EWOULDBLOCK && errno!=EINTR)
//		{
//			return false;
//		}
	}
	return true;
}
bool	DownloadFile::expect(const char(&ret_code)[4],std::string&str)
{
	char buf[512];
	int len=0;
	while(true)
	{
		int s=::recv(cmd_fd,buf+len,sizeof(buf)-len,0);
		if(s==-1)
		{
			LOG_ERROR("recv error[%d]%s",errno,strerror(errno));
			return false;
		}
		else if(s==0)
		{
			LOG_INFO("connection closed by ftp");
			return false;
		}
		len+=s;
		for(int i=len-s;i<len;i++)
		{
			if(buf[i]=='\n' || buf[i]=='\0')//has received a total line
			{
				goto receive_done;
			}
		}
	}
	receive_done:
	LOG_INFO("expect result:%s",buf);

	str=buf;
	bool ret=false;
	int clen=strlen(ret_code);
	for(int i=0;i<str.size();i++)
	{
		if(strncmp(ret_code,buf+i,clen)==0)
		{
			return true;
		}
	}
	return ret;
}
} /* namespace fdl */
