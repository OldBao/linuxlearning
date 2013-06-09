common_srcs = $(wildcard *.cc)
common_dir  = $(notdir $(common_srcs))
common_objs = $(patsubst %.cc,%.o,$(common_dir))


.PHONY: all test
all : lib

lib : $(common_objs)
	@echo "Making $(common_objs)"

test : lib

clean : 
	rm -f *.o
%.o : %.cc
	$(CPP) $(CPPFLAGS) $(INCLUDES) -c -o $@ $^
