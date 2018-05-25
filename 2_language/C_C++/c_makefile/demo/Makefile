
#MYSQL_INC=$(shell mysql_config --include)
#MYSQL_LIB=$(shell mysql_config --libs_r)
#------------------------------------------sources
SRC=$(wildcard \
	src/*.cpp	\
	xml/*.cpp	\
	)
#------------------------------------------headers
HEADER_PATH=\
	.	\
	headerpath1	\
	headerpath2	\
	
#------------------------------------------libs
LIB= dl mysqlclient \
		protobuf	\
					
LIB_PATH=\
	/usr/local/lib	\
	/usr/lib64/mysql	\
	
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

TARGET1=bin/elf
TARGET2=bin/monitor
all:$(TARGET1) $(TARGET2)
$(TARGET1):$(OBJ)
	$(CXX) -o $@ $(LINKFLAGS) $(INC) $(ULIB_PATH) $^ $(ULIB)
$(TARGET2):$(TARGET1)
	cp -f $(TARGET1) $(TARGET2)

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
	rm -f $(RM_ITEMS) $(TARGET1) $(TARGET2)
	

debug:
	$(info $(SRC))
	$(info $(OBJ))
	$(info $(RM_ITEMS))
