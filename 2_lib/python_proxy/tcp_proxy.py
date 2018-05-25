import select
import socket
import time
local_addr=('0.0.0.0',8089)
cdn_addr=('127.0.0.1',80)
socket_fd=socket.socket(socket.AF_INET,socket.SOCK_STREAM);
socket_fd.bind(local_addr);
socket_fd.listen(1);
has_conn=False;
while True:
	if not has_conn:
		cli_fd,cli_addr=socket_fd.accept();
		has_conn=True;
		cdn_fd=socket.socket(socket.AF_INET,socket.SOCK_STREAM);
		cdn_fd.connect(cdn_addr);
		print 'new connection, connected to cdn\n'
	inputs=[cli_fd,cdn_fd];
	r,w,e=select.select(inputs,[],[],20);
	if not r:
		continue
	for s in r:
		print 'in for loop\n'
		if s is cli_fd:
			try:
				data=cli_fd.recv(10000);
				if not data:
					print 'recv client not data\n'
					cli_fd.close();
					cdn_fd.close();
					has_conn=False;
				else:
					print 'recv client data\n'
					cdn_fd.send(data);
			except:
				print 'exception occurred'
				cli_fd.close();
				cdn_fd.close();
				has_conn=False;
				
		if s is cdn_fd:
			try:
				data=cdn_fd.recv(10000);
				if not data:
					print 'recv cdn not data\n'
					cdn_fd.close();
					cli_fd.close();
					has_conn=False;
				else:
					print 'recv cdn data\n'
					cli_fd.send(data);
			except:
				print 'exception occurred'
				cli_fd.close();
				cdn_fd.close();
				has_conn=False;
	

