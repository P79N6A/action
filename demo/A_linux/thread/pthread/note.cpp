

//write signal function here
void mysignal(int x)
{
	LOG_DEBUG("signal:%d received by thread:%ld, pthread_exit will be called",x,(long)pthread_self());
	pthread_exit(0);
}
//===============================================
//register signal function
{//
	if(signal(SIGTERM,mysignal)==SIG_ERR)
	{
		printf("signal called error[%d]%s\n",errno,strerror(errno));
		return -1;
	}
}
//===============================================
//create thread
if(int err=pthread_create(&task->thread_id,0,thread_fun_function,task))
{
	LOG_ERROR("pthread crerate error:%d",err);
	return ERROR_CREATE_THREAD_FAILED;
}

//thread_run_function

void* thread_fun_function(void*arg)
{
	Task*task=(Task*)arg;
	
	do_task();
	task->thread_id=0;
	task->thread_has_exit=true;
	LOG_INFO("thread:%ld of  task:%llu will exit",(long)task->thread_id,task->pf.task_index_key);
	return arg;
}


//kill thread
if(int ret=pthread_kill(task->thread_id,SIGTERM))
{
	LOG_ERROR("kill thread:%ld error",(long)task->thread_id);
}

//try join after killing thread or thread ran over;
if(r=pthread_tryjoin_np(task->thread_id,0))
{
	//..do nothing
	if(abs(CTS-task->kill_time)>=5)//if tryjoin for over 5 seconds, this task should also be recycled
	{
		tryjoin_succ=false;
		r=0;
	}
}


