import socket
addr=('0.0.0.0',31483)
sockfd=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
sockfd.bind(addr)
recv_num=0
while True:
	recv_data,peer_addr=sockfd.recvfrom(10000)
	if not recv_data:
		print peer_addr,'exit'
		break
	recv_num+=1
	print peer_addr,'send------------------------------------------------>',recv_num,'\n',recv_data
sockfd.close()

