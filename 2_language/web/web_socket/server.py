
log_file_path='''F:\\udp_game\\pc\\Cache\\apollo.log'''
######################################################################################
import socket
import time
import select
import datetime
import json
from threading import Thread
import Queue
import re
from collections import deque
from SimpleWebSocketServer import WebSocket
from tail import Tail
#import pdb
#pdb.set_trace()
######################################################################################
class Pack:
	def __init__(self,aTime=0.0,aLen=0):
		self.mTime=aTime
		self.mLen=aLen
class StaticValue:
	def __init__(self,aTime=0.0,aValue=0):
		self.mTime=aTime
		self.mValue=aValue

class Speed:
	def __init__(self,aTime,aSpeed=0,aNum=0):
		self.mTime=aTime
		self.mSpeed=aSpeed
		self.mNum=aNum
	def addPack(self,aLen):
		self.mSpeed+=aLen
		self.mNum+=1

def get_time_from_line(line):

#	print 'line',line
	words=line.split()
	time_ymd=words[0][1:]
	time_hms=words[1]
	r=datetime.datetime(year=int(time_ymd[0:4]),month=int(time_ymd[5:7]),day=int(time_ymd[8:10]),hour=int(time_hms[0:2]),minute=int(time_hms[3:5]),second=int(time_hms[6:8]))
#	print 'after_datetime()',r
	return r
match_send_line=re.compile(""".*\[send_tcp_debug_print\].*""")
match_recv_line=re.compile(""".*\[recv_tcp_debug_print\].*""")
match_duplicate=re.compile(""".* duplicate .*""")
match_tscalc=re.compile(""".* TSCalc .*""")
match_time=re.compile('''\[2015-.*''')

result={'send_speed':0,'recv_speed':0}
sendSpeed=Speed(datetime.datetime(2015,7,5,0,0,0))
recvSpeed=Speed(datetime.datetime(2015,7,5,0,0,0))
BRIDGE=Queue.Queue()
def deal_line(line):
#	print 'line',line
	global match_send_line,match_recv_line,match_duplicate,match_tscalc,match_time
	global last_time
	global sendSpeed,recvSpeed


	if not line:
		return

	if match_send_line.match(line):
		aTime=get_time_from_line(line)
		aLen=line.split()[-1][5:-1]
#		print 'aTime:',aTime
#		print 'aLen:',aLen
		if (aTime.hour==sendSpeed.mTime.hour and aTime.minute==sendSpeed.mTime.minute and aTime.second==sendSpeed.mTime.second):
#			print 'in_if_A'
#			print 'send_time:',sendSpeed.mTime
#			print 'send_speed:',sendSpeed.mSpeed
#			print 'send_num:',sendSpeed.mNum
			sendSpeed.addPack(float(aLen))
#			print 'in_if_B'
#			print 'send_time:',sendSpeed.mTime
#			print 'send_speed:',sendSpeed.mSpeed
#			print 'send_num:',sendSpeed.mNum
		else:
#			print 'in_else'
			result['send_speed']=sendSpeed.mSpeed
			result['send_num']=sendSpeed.mNum
			BRIDGE.put(result)
			sendSpeed=Speed(aTime)
		
#		display_BRIDGE()

	elif match_recv_line.match(line):
		aTime=get_time_from_line(line)
		aLen=line.split()[-1][5:-1]
		if(aTime.hour==recvSpeed.mTime.hour and aTime.minute==recvSpeed.mTime.minute and aTime.second==recvSpeed.mTime.second):
			recvSpeed.addPack(float(aLen))
		else:
			result['recv_speed']=recvSpeed.mSpeed
			result['recv_num']=recvSpeed.mNum
			BRIDGE.put(result)
			recvSpeed=Speed(aTime)
#		display_BRIDGE()
#	print result
#	display_BRIDGE()

#	if match_time.match(line):
#		t=get_time_from_line(line)
#		print 't->',t


#	elif match_duplicate.match(line):
#		aTime=get_time_from_line(line)
#		dup_count=dup_count+1
#		duplicate.append(StaticValue(aTime,int(dup_count)))
#	elif match_tscalc.match(line):
#		aTime=get_time_from_line(line)
#		t = line.split()[-1][1:-1]
#		tscalc.append(StaticValue(aTime,int(t)))
def display_BRIDGE():
#	pass
	if BRIDGE.empty():
		print 'empty'
	else:
		print 'not empty\n'
		
#		a=BRIDGE.get()	
#		print '\na',a['send_speed']
#		print '\na',a['recv_speed']

#	for m in BRIDGE:
#		print m['recv_speed'],'---',m['send_speed']
#######################################################################################
trace_log=Tail(log_file_path)
trace_log.register_callback(deal_line)

#trace_log.follow(s=0.01)
def thread_fun():
	print 'thread fun start'
#	for i in range(100):
#		BRIDGE.put(i)
	trace_log.follow(s=0.01)
	print 'thread fun ended'

trace_thread=Thread(target=thread_fun)
trace_thread.start()

def get_send_data():
	a=''
#	a=BRIDGE.get()
	while True:
		a=BRIDGE.get()
		if BRIDGE.empty():
			break
		
#	display_BRIDGE()
#	print 'a:',a
#	print '\nsend_speed',a['send_speed']
#	print '\nrecv_speed',a['recv_speed']
	return json.dumps(a)

#for i in range(1000000):
#	x=get_send_data()
#	print 'x:',x
#######################################################################################
CLOSE = 0x8

listen_socket=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
#listen_socket.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
listen_socket.bind(('127.0.0.1',1507))
listen_socket.listen(1)

sock,addr=listen_socket.accept()
print 'connected by:',addr

client=WebSocket(0,sock,addr)
print 'Websocket created'
Listeners=[sock.fileno()]
connections={}
connections[sock.fileno()]=client
send_time=datetime.datetime.now()
send_tick=datetime.timedelta(seconds=1)
while True:
	rList,wList,xList=select.select(Listeners,Listeners,Listeners,7);
	if not (rList or wList or xList):
		continue
	
	for r in rList:
		zc=connections[r]
		zc._handleData()
		while zc.sendq:
			opcode, payload = zc.sendq.popleft()
			if opcode == CLOSE:
				raise Exception("received client close")
			remaining = zc._sendBuffer(payload)
			if remaining is not None:
				zc.sendq.appendleft((opcode, remaining))
				break

	for w in wList:
		time_now=datetime.datetime.now()
		if time_now>send_time+send_tick:
			print time_now
			send_time=send_time+send_tick
			zw=connections[w]
			get_send_data()
			zw.sendMessage(unicode(get_send_data(),'utf-8'))

			opcode, payload = zw.sendq.popleft()
			remaining = zw._sendBuffer(payload)
			if remaining is not None:
				zc.sendq.appendleft((opcode, remaining))
				break
######################################################################################
BRIDGE.join()