import socket
import time
ba=('172.28.0.68',30000)

da1 = ('169.50.48.205',5555)
da2 = ('169.50.48.194',5555)

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF,650000)
#s.setblocking(0)
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

        if send_num<200000:
                r=s.sendto(data, da1)
                if r!=200:
                        print 'send to error',r
                send_num+=1;
                r=s.sendto(data, da2)
                if r!=200:
                        print 'send to error',r
                send_num+=1;

        now=time.time();
        if now-last>=1:
                last=now;
                print now,'send_num:',send_num;
                send_num=0;
#       time.sleep(1);

s.close()