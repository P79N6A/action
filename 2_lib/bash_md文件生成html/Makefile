#makefile   
MD = markdown2  
#SOURCES := $(wildcard *.md)  
#OBJECTS := $(patsubst %.md, %.html, $(wildcard *.md))  
  
TOPDIR := $(shell pwd)  
DIR := $(shell find $(TOPDIR) -type d|egrep -v '/\.git|/all')  
SOURCES := $(foreach subdir,$(DIR),$(wildcard $(subdir)/*.md))  
OBJECTS := $(patsubst %.md, %.html, $(SOURCES))  
  
all:html suffix  index.html  
  
html: $(OBJECTS)  
  
$(OBJECTS): %.html: %.md  
	@set -e; \
	sed '1i\<meta http-equiv="content-type" content="text/html; charset=UTF-8">' $< > $@.t; \
	$(MD) --encoding=UTF-8 $@.t > $@; \
	rm -f $@.t;  	

#	sed '1i\<meta http-equiv="content-type" content="text/html; charset=UTF-8">' $< > $@.t; \
#	$(MD)  $@.t > $@ ;    \
#	rm -f $@.t;  	

suffix:  
	sed -i '/.md/s/md/html/g' $(OBJECTS)  
  
index.html:$(OBJECTS)
	sh gen_content.sh
clean:  
	rm -f $(OBJECTS)  
debug:
	$(info $(TOPDIR))
	$(info $(DIR))
	$(info $(SOURCES))
#       $(info $(OBJECTS))
