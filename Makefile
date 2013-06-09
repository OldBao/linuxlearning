TOPDIR=$(CURDIR)
INCLUDES = -I$(CURDIR)/include 

AR=ar
TARGET_LIB=libll.a

CPP=g++
ifeq ($(MAKECMDGOALS), test)
CPPFLAGS = -O0 -g -DDEBUG
else
CPPFLAGS = -O2 -DNDEBUG
endif

objs = ds/*.o

.PHONY : all test lib

SUBDIRS = test

CPPFLAGS += -pipe -Wall  -finline-functions

LDFLAGS  += -lpthread -lrt -lm

all :
	$(MAKE) lib

	@echo "Done Forget run make test"

lib: 
	make -C ds
	ar rcs $(TARGET_LIB) $(objs)
	mv $(TARGET_LIB) lib/

test : $(test_objs) lib
	$(MAKE) -C test

clean: 
	rm  -f lib/$(TARGET_LIB)
	$(MAKE) -C test clean
	$(MAKE) -C common clean
	$(MAKE) -C ds clean

%.o : %.cc
	$(CPP) $(CPPFLAGS) $(INCLUDES) -c -o $@ $^

export
