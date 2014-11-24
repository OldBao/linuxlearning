all:

SUBDIRS = common . test

AM_CPPFLAGS = -I$(top_srcdir)/include

lib_LTLIBRARIES = libll.la
libll_la_SOURCES = ds/ll_vector.cc ds/ll_rbtree.cc common/ll_log.cc events/ll_events.cc events/ll_epoll_events.cc

libll_la_CXXFLAGS =  -fPIC
libll_la_LDFLAGS = -lm -lrt -lpthread  -shared -fPIC

bin_PROGRAMS=event_main
event_main_SOURCES=events/events_main.cc
event_main_LDFLAGS=-lll -lm -lrt -lpthread 
