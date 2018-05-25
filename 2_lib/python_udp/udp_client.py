import socket
import time
ba=('127.0.0.1',20002)

da = ('127.0.0.1',6821)

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(ba);
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)  

data='1'*200;
print 'string:',data


n=10000000
recv_num=0;
send_num=0;
last=0;
while True:
	n=n-1

	if send_num<40000:
		s.sendto(data, da)
		send_num+=1;

	now=time.time();	
	if now-last>=1:
		last=now;
		print now,'send_num:',send_num;
		send_num=0;
#	time.sleep(1);

s.close()
