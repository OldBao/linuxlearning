all:
AM_CPPFLAGS = -I$(top_srcdir)/include

lib_LTLIBRARIES = libll.la
libll_la_SOURCES = ll_vector.cc ../common/ll_log.cc
libll_la_CXXFLAGS = `pkg-config libgvc --cflags` -fPIC
libll_la_LDFLAGS = -lm -lrt -lpthread `pkg-config libgvc --libs` -shared -fPIC