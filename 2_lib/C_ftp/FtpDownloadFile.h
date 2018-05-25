
#ifndef OWNLOADFILE_H_
#define OWNLOADFILE_H_
#include<fstream>
#include"base.h"
namespace fdl {
struct NetAddr
{
	uint32	ip;
	uint16	port;
	char	str_addr[64];
};
class DownloadFile {
	char	ftp_user[NAME_LEN];
	char	ftp_pass[NAME_LEN];
	NetAddr	ftp_addr;

	int cmd_fd;
	int data_fd;
	std::fstream outfile;
public:
	DownloadFile();
	virtual ~DownloadFile();
	bool	init(const char*user,const char*password,const char*url);
	bool	download(const char*file,char(&out_file)[256]);
	bool	create_socket(int*fd);
	bool	command(const char*buf);
	bool	expect(const char(&ret_code)[4],std::string&str);
};

} /* namespace fdl */

#endif /* OWNLOADFILE_H_ */
