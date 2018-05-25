#Makefile for voice_server
#MYSQL_INC=$(shell mysql_config --include)
#MYSQL_LIB=$(shell mysql_config --libs_r)
#------------------------------------------sources
SRC=$(wildcard \
	./*.cpp	\
	)
#------------------------------------------headers
HEADER_PATH=\
	.	\
	
#------------------------------------------libs
LIB= 	\
			
			
LIB_PATH=\
	.	\
	
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

TARGET1=bin/pp_common.lib
all:$(TARGET1) $(TARGET2)
$(TARGET1):$(OBJ)
	$(AR) rs $(TARGET1) $^
#	$(CXX) -o $@ $(LINKFLAGS) $(INC) $(ULIB_PATH) $^ $(ULIB)

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
	
tag_name=$(shell echo "TAG_MAME_2_0_2_"`date +"%Y%m%d-%H%M%S"`)
tag:
	cat  git_tag.h | sed 's@\"\[.*\]\"@\"\[$(tag_name)\]\"@g' > tmpfile	
	mv tmpfile git_tag.h
	
#	rm -f $(RM_ITEMS) $(TARGET1) $(TARGET2)
	git add git_tag.h
	sleep 0.1
	git commit -m "add git_tag.h"
	sleep 0.1
	git push
	sleep 0.1
	git tag -a $(tag_name)
	sleep 0.1
	git push origin $(tag_name)
pack:
	rm -rf bin/log
	rm -rf TAG_MAME*
	cp -r bin $(tag_name)
	tar -zcvf $(tag_name)".tar.gz" $(tag_name)
debug:
	$(info $(SRC))
	$(info $(OBJ))
	$(info $(RM_ITEMS))
