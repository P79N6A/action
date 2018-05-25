BEGIN {
	FS="|" 
	system("rm unnormal_items.dat")
}
NR==1 {
	print NF
	for(i=0;i<=NF;i++)
	{
		print i,$i
	}
}

END {
	printf("TOTAL NUMBER:%d\n",NR);
	printf("=====================================================================\n");
}
{#connection_live_time_ms
	DIST_INTERVAL=20;
	k=int($14/60/1000);
#	print k
	if(k>DIST_INTERVAL) k=DIST_INTERVAL;
	live_time[k]++;
}
END {
	printf("\nCONNECTION_LIVE_TIME--------->\n");
	for(i=0;i<DIST_INTERVAL;i++)
	{
		printf("[%d~%d] %.2f% (%d)\t",i,i+1,live_time[i]*100/NR,live_time[i]);
	}
	printf("[>=%d] %.2f% (%d)\n",i,live_time[i]*100/NR,live_time[i]);
}
	
{#close_reason

	close_reason[$15]++
}
END {
	printf("\nCLOSE_REASEON\t---------->\n");
	for(i=0;i<=6;i++)
	{
		printf("[%d] %d\t",i,close_reason[i]);
	}
	printf("\n");
	printf("close_normal\t%.2f% (%d)\nclose_by_peer_fin\t%.2f% (%d)\nclose_by_peer_rst\t%.2f% (%d)\nclose_write_error\t%.2f% (%d)\nclose_keepalive_timeout\t%.2f% (%d)\nclose_socket_write_error\t%.2f% (%d)\ntcp_error_code_end\t%.2f% (%d)\n",
			close_reason[0]/NR*100,close_reason[0],
			close_reason[1]/NR*100,close_reason[1],
			close_reason[2]/NR*100,close_reason[2],
			close_reason[3]/NR*100,close_reason[3],
			close_reason[4]/NR*100,close_reason[4],
			close_reason[5]/NR*100,close_reason[5],
			close_reason[6]/NR*100,close_reason[6]);
}
{#rtt
	if($55)
	{
		sample_num++;
		big_rtt=0
		for(i=59;i<=62;i++)
		{
			big_rtt+=$i;
		}	
		total_big_rtt_percent+=big_rtt/$55;
	}
	else
	{
		print($0)>>"unnormal_items.dat"
		unnormal_sample_num++;
	}
}
END {
	printf("\nRTT\t---------->\n");
	printf("samle_num\t%d\nunnormal_sample\t%d\naverage_big_rtt_percent\t%.2f% (%d)\n",
			sample_num,
			unnormal_sample_num,
			total_big_rtt_percent/sample_num*100,
			total_big_rtt_percent);
}
{#whether_one_message_per_mobile
	mobile[$114]++
}
END {
	printf("\nWHTEHTER_ONE_MESSAGE_PER_MOBILE\t---------->\n");

	max_item_num=0;
	for(m in mobile)
	{
		if(mobile[m]>max_item_num)
		{
			max_item_num=mobile[m];
			max_id=m
		}
	}
	printf("max_id\t%s\nmax_item_num\t%d\n",max_id,max_item_num);
}
#connection_write_errno
{
	if($16)
	{
		write_errno[$16]++;
	}
}
END {
	printf("\nCONNECTION_WRITE_ERRNO\t----------->\n");
	for(w in write_errno)
	{
		printf("[%d] %.2f% (%d)\t",w,write_errno[w]/NR*100,write_errno[w]);
	}
	print("\n")
}
#pcb_state
{
	pcb_state[$17]++;
}
END {
	printf("\nPCB_STATE\t---------->\n");
	for(p in pcb_state)
	{
		printf("[%d] %.2f% (%d)\t",p,pcb_state[p]/NR*100,pcb_state[p]);
	}
	print("\n")
}
#send_error101
{
	send_error101[$20]++;
}
END {
	printf("\nSEND_ERROR101\t------>\n");
	#slen=asorti(send_error101,se);
	for(i=1;i<=slen;i++)
	{
		printf("[%d] %.2f% (%d)\t",se[i],send_error101[se[i]]/NR*100,send_error101[se[i]]);
	}
	print("\n")
}
#send_error11
{
	send_error11[$21]++;
}
END {
	printf("\nSEND_ERROR11\t---------->\n");
	#slen=asorti(send_error11,se);
	for(i=1;i<slen;i++)
	{
		printf("[%d] %.2f% (%d)\t",se[i],send_error11[se[i]]/NR*100,send_error11[se[i]]);
	}
	print("\n")
}
