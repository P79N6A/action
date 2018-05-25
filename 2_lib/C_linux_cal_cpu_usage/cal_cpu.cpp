
#include<unistd.h>
#include<stdio.h>
#include"cal_cpu.h"
namespace vs{
unsigned cal_cpu_usage(unsigned now)
{
	unsigned ret=100;

	pid_t pid=getpid();
	char buf[1024];
	snprintf(buf,sizeof(buf),"/proc/%d/stat",(int)pid);
	FILE*fd=fopen(buf,"r");
	if(!fd)
	{
		LOG_ERROR("open file error:%s",buf);
		return ret;
	}
	int v1;
	char v2[128];
	char v3;
	int v4,v5,v6,v7,v8;
	unsigned v9;
	unsigned long v10,v11,v12,v13,v14,v15;
	if(!fgets(buf,sizeof(buf),fd))
	{
		LOG_ERROR("fgets error");
		return ret;
	}
	LOG_DEBUG("read buf:%s",buf);
	fclose(fd);

	sscanf(buf,"%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu",&v1,v2,&v3,&v4,&v5,&v6,&v7,&v8,&v9,&v10,&v11,&v12,&v13,&v14,&v15);
	unsigned sc_clk_tck=sysconf(_SC_CLK_TCK);
	if(sc_clk_tck>1000)
	{
		LOG_ERROR("the configured _SC_CLK_TCK is  too big so we can't use it for calculation");
		return ret;
	}
	if(sc_clk_tck==0)
	{
		LOG_ERROR("_SC_CLK_TCK can't be zero");
		return ret;
	}
	unsigned tic_time=1000/sc_clk_tck;
	static unsigned long last_tic=(v14+v15)*tic_time;
	unsigned long now_tic=(v14+v15)*tic_time;
	int diff_tic=now_tic-last_tic;
	last_tic=now_tic;

	static unsigned last_time_ms=now;
	unsigned diff_time=now-last_time_ms;
	last_time_ms=now;

	ret=diff_time?(diff_tic*100/diff_time):100;
	LOG_DEBUG("the cpu usage is :%u",ret);
	return ret;
}


}


