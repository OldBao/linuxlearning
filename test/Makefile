LIBRARY_PATH=-L$(TOPDIR)/lib
LIBS = -lgtest -lgtest_main -lll

test_srcs=$(wildcard *.cc)
test_dir=$(notdir $(test_srcs))
test_objs=$(patsubst %.cc,%.o, $(test_dir))

OUT = test_main

.PHONY: all test
all :  test

test : $(test_objs) 
	$(CPP) $(CPPFLAGS) -o $(OUT) $^ $(LIBRARY_PATH) $(LIBS) $(LDFLAGS)

clean:
	rm -f $(OUT) 
	rm -f *.o

%.o : %.cc
	$(CPP) $(CPPFLAGS) $(INCLUDES) -c -o $@ $^

export
