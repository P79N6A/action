all:src dst
src:src.cpp ../detect_alive_udp.h ../../C_pp_common/pp_socket.h
	g++ -g -o $@ -I../../C_pp_common -I.. $<
dst:dst.cpp ../detect_alive_udp.h ../../C_pp_common/pp_socket.h
	g++ -g -o $@ -I../../C_pp_common -I.. $<
.PHONY:all clean
clean:
	rm -rf src dst *.log
	killall src dst
