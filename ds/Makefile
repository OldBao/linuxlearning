ds_srcs = $(wildcard *.cc)
ds_dir  = $(notdir $(ds_srcs))
ds_objs = $(patsubst %.cc,%.o,$(ds_dir))

.PHONY: all test lib
all : lib

lib : $(ds_objs)

test : lib

clean : 
	rm -f *.o

%.o : %.cc
	$(CPP) $(CPPFLAGS) $(INCLUDES) -c -o $@ $^
