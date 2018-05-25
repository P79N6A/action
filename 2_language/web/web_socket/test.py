from collections import deque
import itertools
import socket
import datetime
import time
#a=[1,2,3,4]
#b=iter(a)
##for i in range(len(a)):
##	print b.next()
##else:
##	print 'the loop is over'
#
#
#c=deque(itertools.islice(b,3))
#print c
#print '----------------------------------\n'
#print isinstance('abc',unicode)
#bb=bytearray('abc')
#print isinstance(bb,unicode)
#print '----------------------------------\n'
#def fun():
#	return 3,4
#ca=fun()
#print ca
#
#print '----------------------------------\n'
#da=socket.socket(socket.AF_INET,socket.SOCK_STREAM);
#print da
#print da.fileno()
#print da==da.fileno()
#print '----------------------------------\n'
e=datetime.datetime.now()
tick=datetime.datetime(2015,1,1,hour=10,minute=10,second=10)
print tick.hour
#print tick.seconds
#print e.seconds
#ea=e+tick
#print e
#print ea
#for i in range(10):
#	print ea;
#	ea=ea+tick
#print ea<ea+tick
#print ea>ea-tick
#print '----------------------------------\n'
##function_name='gun'
#def fun():
#	global fa
#	print fa
#	gun()
#
#fun()
#
#def gun():
#	print 'gun'
#fa='fa'
#from collections import deque
#import Queue
#from threading import Thread
#import time
#from tail import Tail
#
#def producer(out_q):
#    time.sleep(1)
#    out_q.put('hello')
#
#def consumer(in_q):
#    h = in_q.get()
#    print(h + ' world!')
#    in_q.task_done()
#
#q = Queue.Queue()
#t1 = Thread(target=producer, args=(q,))
#t2 = Thread(target=consumer, args=(q,))
#
#t1.start()
#t2.start()
#
#q.join()
#print('q joined!')
#
#
#def print_line(txt):
#    ''' Prints received text '''
#    print(txt)
# 
## Create a tail instance
#t = Tail(r'a.txt')
# 
## Register a callback function to be called when a new line is found in the followed file.
## If no callback function is registerd, new lines would be printed to standard out.
#t.register_callback(print_line)
# 
## Follow the file with 2 seconds as sleep time between iterations.
## If sleep time is not provided 1 second is used as the default time.
#t.follow(s=0.01)