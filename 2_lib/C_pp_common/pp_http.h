/*
 * pp_http.h
 *
 *  Created on: Apr 19, 2019
 *      Author: purperzhang
 */


#ifndef COMMON_PP_COMMON_PP_HTTP_H_
#define COMMON_PP_COMMON_PP_HTTP_H_

#include<string>
#include<map>
#undef max
#undef min
#include<sstream>
namespace pp{

class HttpPaser
{
public:
	int limited_http_length;
	int predict_content_len;
	int head_len;
	int content_len;
	std::string content;
	int parse_len;
	std::map<std::string,std::string> head;
	HttpPaser(){
		limited_http_length=1000000;
		predict_content_len=1024;
		head_len=0;
		content_len=0;
		parse_len=0;
	}
	void	set_limit_http_length(int a)
	{
		limited_http_length=a;
	}
	void	set_predict_content_length(int a)
	{
		predict_content_len=a;
	}
	bool	parse_full_data(const char*data,int data_len)
	{
		if(data_len>limited_http_length)
		{
			LOG_ERROR("too long data_len=%d",data_len);
			return false;
		}
		head.clear();
		//get first line
		const char*p=data;
		std::string i;

#define A(X) if(!(X)) return false
#define L(X) if(!(X) || end_num<1) return false
		int end_num=0;

		A(get_item(p,i,' '));
		head["method"]=i;
		A(get_item(p,i,' '));
		head["uri"]=i;
		L(this->get_line(p,i,&end_num));
		head["version"]=i;

		do{
			std::string k,v;
			A(get_item(p,k,':'));
			L(this->get_line(p,v,&end_num));
			head[k]=v;
		}while(end_num==1);
		if(end_num<=0)
		{
			//data not enough
			return false;
		}

		head_len=p-data;

		if(end_num==2)
		{
			content_len=atoi(head["Content-Length"].c_str());
			if(content_len==0)
			{
				return false;
			}
			if(head_len+content_len>data_len)
			{
				//data not enough
				return false;
			}
//			if(head_len+content_len<data_len)
//			{
//				LOG_ERROR("too long length, head_len:%d body_len:%d,data_len:%d",head_len,content_len,data_len);
//				return false;
//			}
			content.clear();
			content.reserve(predict_content_len);
			int cl=content_len;
			while(*p && (cl--)>0)
			{
				content.push_back(*(p++));
			}
			if(!*p && cl!=0)
			{
				//hasn't receive enough data
				return false;
			}
			parse_len=p-data;
			return true;
		}
		else
		{
			LOG_ERROR("wrong format,too many end line break=%d",end_num);
			return false;
		}
#undef A
#undef L

	}
	bool get_item(const char*&p,std::string& line,char delim)
	{
		line.clear();
		while(true)
		{
			if(!*p)
				break;
			if(*p==delim)
			{
				while(*p==delim)
				{
					p++;
				}
				break;
			}
			line.push_back(*p);
			p++;
		}
		if(!*p && line.empty())
			return false;
		return true;
	}
	bool get_line(const char*&p,std::string& line,int*line_end_num)
	{
		line.clear();
		while(true)
		{
			if(!*p)
			{
				*line_end_num=0;
				break;
			}
			if(*p=='\r' || *p=='\n')
			{
				*line_end_num=lb_num(p);
				break;
			}
			line.push_back(*p);
			p++;
		}
		if(!*p && line.empty())
			return false;
		return true;
	}
	int lb_num(const char*&p)
	{
		int n=0;
		while(*p=='\r' || *p=='\n')
		{
			if(*p=='\n')
				n++;
			p++;
			if(!*p)
				break;
		}
		return n;
	}
	string display_head()
	{
		using std::endl;
		std::stringstream ss;
		ss<<"head_len \t=>"<<head_len<<endl
				<<"body_len \t=>"<<content_len<<endl;
		for(map<string,string>::iterator p=head.begin();p!=head.end();p++)
		{
			ss<<p->first<<" \t=>"<<p->second<<endl;
		}
		return ss.str();
	}
};

}//namespace pp

#endif /* COMMON_PP_COMMON_PP_HTTP_H_ */
