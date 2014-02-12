AM_CPPFLAGS = -I$(top_srcdir)/include

bin_PROGRAMS = test_main
test_main_SOURCES = test_list.cc test_vector.cc test_main.cc test_rbtree.cc
#test_main_LDFLAGS = -L$(top_srcdir)/ds/.libs/ -L../lib -lgtest -lpthread -lm -lll 
test_main_LDFLAGS =  -L../lib -L$(top_srcdir)/ds/ -lgtest -lpthread -lm -lll
