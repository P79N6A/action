#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/epoll.h>
#include<curl/curl.h>
#include<fstream>
using namespace std;
#define LOG(format,args...) printf("[%d] "format"\n",__LINE__,##args)
typedef struct _global_info {
	int epfd;
	CURLM *multi;
} global_info;

typedef struct _easy_curl_data {
	CURL *curl;
	char data[1024];
} easy_curl_data;

typedef struct _multi_curl_sockinfo {
	curl_socket_t fd;
	CURL *cp;
} multi_curl_sockinfo;

char curl_cb_data[1024] = {0};
static int multi_timer_cb(CURLM *multi, long timeout_ms, global_info *g)
{
	
	LOG("timer cb called");
	  return 0;
}

//static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
//{
//  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
//  return written;
//}
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	LOG("write data called");
	int n=size*nmemb;
	for(int i=0;i<n;i++)
	{
		cout<<((char*)ptr)[i];
	}
//exit(0);
  return n;
}
static int sock_cb (CURL *e, curl_socket_t s, int what, void *cbp, void *sockp)
{
	struct epoll_event ev = {0};

	global_info * g = (global_info *) cbp;
	multi_curl_sockinfo  *fdp = (multi_curl_sockinfo *) sockp;

	if (what == CURL_POLL_REMOVE) {
		LOG("CURL_POLL_REMOVE called");
	    if (fdp) {
	        free(fdp);
	    }
	    epoll_ctl(g->epfd, EPOLL_CTL_DEL, s, &ev);//the last parameter in linux2.6.9 should not be null, though encounters EPOLL_CTL_DEL
	} else {
//	    if (what == CURL_POLL_IN) {
//	        ev.events |= EPOLLIN;
//		printf("%s\n",curl_cb_data);
//	    } else if (what == CURL_POLL_OUT) {
//	        ev.events |= EPOLLOUT;
//	    } else if (what == CURL_POLL_INOUT) {
//	        ev.events |= EPOLLIN | EPOLLOUT;
//	    }

        ev.events |= (EPOLLIN | EPOLLOUT);
	ev.data.fd=s;
	    if (!fdp) {
	        fdp = (multi_curl_sockinfo *)malloc(sizeof(multi_curl_sockinfo));
	        fdp->fd = s;
	        fdp->cp = e;

	        epoll_ctl(g->epfd, EPOLL_CTL_ADD, s, &ev);
	        curl_multi_assign(g->multi, s, fdp);
	    }

	}
	return 0;
}


int main(int argc, char *argv[])
{
  static const char *pagefilename = "page.out";
  FILE *pagefile;
	LOG("this protram is:%s",argv[0]);
	/*normal called*/
	curl_global_init(CURL_GLOBAL_ALL);
	/*prepare variable*/
	global_info g;
	memset(&g, 0, sizeof(global_info));
	g.epfd = epoll_create(10);
	g.multi = curl_multi_init();
	//char *urls[1] = {"http://www.beautifulunknown.top"};
//	char *urls[1] = {"http://www.baidu.com"};
	char *urls[2] = {"http://172.19.18.237:80","http://www.baidu.com"};
	
	int i=0;
	for(;i<2;i++) {
	    CURL *curl;
	    curl = curl_easy_init();
	    curl_easy_setopt(curl, CURLOPT_URL, urls[i]);
	    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
	    		curl_easy_setopt(curl,CURLOPT_VERBOSE,1L);
	    		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,write_data);
	    		curl_easy_setopt(curl, CURLOPT_WRITEDATA, curl_cb_data);
	    //		curl_easy_setopt(curl,CURLOPT_TIMEOUT,30);
//	    /* Switch on full protocol/debug output while testing */
//	    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
//
//	    /* disable progress meter, set to 0L to enable and disable debug output */
//	    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
//
//	    /* send all data to this function  */
//	    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
//
//	    /* open the file */
//	    pagefile = fopen(pagefilename, "wb");
//	    /* write the page body to this file handle */
//	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, pagefile);



	    if(int r=curl_multi_add_handle(g.multi, curl))
	    {
		    LOG("error:%d",r);
		    return -1;
	    }
	}

	if(int r=curl_multi_setopt(g.multi, CURLMOPT_SOCKETFUNCTION, sock_cb))
	{
	    LOG("error:%d",r);
		return -1;
	}
	if(int r=curl_multi_setopt(g.multi, CURLMOPT_SOCKETDATA, &g))
	{
	    LOG("error:%d",r);
		return -1;
	}
	curl_multi_setopt(g.multi, CURLMOPT_TIMERFUNCTION, multi_timer_cb);
	  curl_multi_setopt(g.multi, CURLMOPT_TIMERDATA, &g);

	int running_count;
	struct epoll_event events[10];
//	if(int r=curl_multi_socket_action(g.multi, CURL_SOCKET_TIMEOUT, 0, &running_count))//this action only memset the timer in the curl source code
//	{
//	    LOG("error:%d",r);
//		return -1;
//	}
	while(true)
	{
		int r=curl_multi_socket_action(g.multi, CURL_SOCKET_TIMEOUT, 0, &running_count);//this action only memset the timer in the curl source code
		if(r!=CURLM_CALL_MULTI_PERFORM) 
			break;
	}
	LOG("running_count:%d",running_count);
	if (running_count) {
	    do {
	        int nfds = epoll_wait(g.epfd, events, 10, 500);
		if(nfds > 0) {
			for (int i=0;i<nfds; i++) {
				if (events[i].events & EPOLLIN) {
					if(int r=curl_multi_socket_action(g.multi,events[i].data.fd, CURL_CSELECT_IN , &running_count))
					{
						LOG("error:%d",r);
						return -1;
					}

//					LOG("running_count:%d",running_count);
				} 
				if (events[i].events & EPOLLOUT) {
					if(int r=curl_multi_socket_action(g.multi, events[i].data.fd, CURL_CSELECT_OUT, &running_count))
					{
						LOG("error:%d",r);
						return -1;
					}
//					LOG("running_count:%d",running_count);
				}
			}
		}
	    } while (running_count);
		LOG("running out of epoll");
	}
	curl_global_cleanup();
	return 0;
}
