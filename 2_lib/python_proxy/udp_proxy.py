import socket
import random
import time
import fcntl,os
addr=('0.0.0.0',8700)
sockfd=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
sockfd.bind(addr)
caddr=();
paddr=("127.0.0.1",8700);
if not caddr:
	print 'good';
#sockfd.close();
#exit();
while True:
		
	recv_data,client_addr=sockfd.recvfrom(10000)
	
	if not recv_data:
		print peer_addr,'exit'
		break
	
	if client_addr!=paddr:
		caddr=client_addr;
		print('recv data from client and send to server');
		sockfd.sendto(recv_data,paddr);
	else:
		print('recv data from server and send to client');
		sockfd.sendto(recv_data,caddr);
		
sockfd.close()
