all:a.exe b.exe
a.exe:a_easy_perform.cpp
	g++ -o $@ -g $^ `curl-config --libs`
b.exe:b_multi_socket.cpp
	g++ -o $@ -g $^ `curl-config --libs`
clean:
	rm -rf *.exe
	rm -rf page.out
