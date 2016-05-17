AM_CPPFLAGS = $(CXX11_FLAGS) -I$(srcdir)/../src
AM_CXXFLAGS = $(PICKY_CXXFLAGS)
LDADD = ../src/libsourdough.a -lpthread

bin_PROGRAMS = ScreamClient ScreamServer 

ScreamClient_SOURCES = ScreamClient.cpp

ScreamServer_SOURCES = ScreamServer.cpp
