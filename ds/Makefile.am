all:
AM_CPPFLAGS = -I$(top_srcdir)/include

lib_LTLIBRARIES = libll.la
libll_la_SOURCES = ll_vector.cc ll_rbtree.cc ../common/ll_log.cc

libll_la_CXXFLAGS =  -fPIC
libll_la_LDFLAGS = -lm -lrt -lpthread  -shared -fPIC