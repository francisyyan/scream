AM_CPPFLAGS = $(CXX11_FLAGS) -I$(srcdir)/../src
AM_CPPFLAGS += $(GLIB_CFLAGS)
AM_CXXFLAGS = $(PICKY_CXXFLAGS)

LDADD = ../src/libsourdough.a -lpthread
LDADD += $(GLIB_LIBS)

bin_PROGRAMS = ScreamClient ScreamServer 

ScreamClient_SOURCES = ScreamClient.cpp ScreamTx.h ScreamTx.cpp \
											 RtpQueue.h RtpQueue.cpp VideoEnc.h VideoEnc.cpp \
											 Packet.h Packet.cpp Timerfd.h Timerfd.cpp
ScreamServer_SOURCES = ScreamServer.cpp ScreamRx.h ScreamRx.cpp \
											 Packet.h Packet.cpp Timerfd.h Timerfd.cpp
