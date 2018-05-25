import socket
import time
addr=('0.0.0.0',8701)
sockfd=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
sockfd.bind(addr)
recv_num=0
last=0;
while True:
        recv_data,peer_addr=sockfd.recvfrom(10000)
        if not recv_data:
                print peer_addr,'exit'
                break
        recv_num+=1

        now=time.time();
        if now-last>=1:
                last=now;    
                print peer_addr,'send------------------------------------------------>',recv_num,'\n'
                recv_num=0
sockfd.close()
