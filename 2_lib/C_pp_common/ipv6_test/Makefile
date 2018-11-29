all:a.out b.out
b.out:server.cpp pp_udp.h
	g++ -g -o $@ $(filter %.cpp,$^) 

a.out:client.cpp pp_udp.h
	g++ -g -o $@ $(filter %.cpp,$^)
.PHONY:clean
clean:
	rm -rf  *.out
