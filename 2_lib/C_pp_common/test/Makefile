
	

#------------------------------------------sources
SRC=$(wildcard \
	./*.cpp	\
	../*.cpp	\
	../../log/*.cpp	\
	)
#------------------------------------------headers
HEADER_PATH=\
	.	\
	..	\
	../../log	\
	../../log/include	\
	
#------------------------------------------libs
LIB=
LIB_PATH=\
	
	
#---------------------------------------------------
SRC_PATH=$(dir $(SRC))
TMP1=$(filter %.c,$(SRC))
TMP2=$(filter %.cc,$(SRC))
TMP3=$(filter %.cpp,$(SRC))

OBJ=$(TMP1:.c=.o) $(TMP2:.cc=.o) $(TMP3:.cpp=.o)
INC=$(addprefix -I,$(HEADER_PATH)) 
ULIB=$(addprefix -l,$(LIB))
ULIB_PATH=$(addprefix -L,$(LIB_PATH))

RM_ITEMS=$(OBJ:.o=.d) $(OBJ:.o=.i) $(OBJ)
CXXFLAGS=-g -O2 -Werror -fPIC
LINKFLAGS=-static-libgcc

all:tcp_client tcp_server
tcp_client:tcp_client.o $(filter-out %tcp_server.o,$(OBJ))
	$(CXX) -o $@ $(LINKFLAGS) $(INC) $(ULIB_PATH) $^ $(ULIB)
tcp_server:tcp_server.o $(filter-out %tcp_client.o,$(OBJ))
	$(CXX) -o $@ $(LINKFLAGS) $(INC) $(ULIB_PATH) $^ $(ULIB)
        

include $(OBJ:.o=.d)
%.d:%.cpp
	@set -e; rm -f $@; \
	g++ $(INC) -MM $< > $@.t; \
	sed 's,$(*F).o[:]*,$*.o $@: ,g' < $@.t > $@; \
	rm -f $@.t
%.o:%.cpp
	g++ -c -o $@ $(CXXFLAGS) $(INC) $<
#	g++ $(CXXFLAGS) $(INC) -E -o $*.i $<
%.d:%.c
	@set -e; rm -f $@; \
	g++ $(INC) -MM $< > $@.t; \
	sed 's,$(*F).o[:]*,$*.o $@: ,g' < $@.t > $@; \
	rm -f $@.t
%.o:%.c
	g++ -c -o $@ $(CXXFLAGS) $(INC) $<
%.d:%.cc
	@set -e; rm -f $@; \
	g++ $(INC) -MM $< > $@.t; \
	sed 's,$(*F).o[:]*,$*.o $@: ,g' < $@.t > $@; \
	rm -f $@.t
%.o:%.cc
	g++ -c -o $@ $(CXXFLAGS) $(INC) $<


.PHONY:all clean debug pack
clean:
	rm -f $(RM_ITEMS) $(TARGET)
pack:
	tar -zcvf `date +%F`"package.tar.gz" bin
debug:
	$(info $(SRC))
	$(info $(OBJ))
	$(info $(RM_ITEMS))