# Use the C++ linker.
LINK.o = $(LINK.cc)

CXXFLAGS := $(CXXFLAGS) -std=c++11 -I include

ifdef BOOST_ROOT
	CXXFLAGS := $(CXXFLAGS) -I $(BOOST_ROOT)/include
	LDFLAGS := $(LDFLAGS) -L $(BOOST_ROOT)/lib
endif

# Linker flags: use Boost.
LDFLAGS := $(LDFLAGS) -l boost_system
LDFLAGS := $(LDFLAGS) -l boost_thread
LDFLAGS := $(LDFLAGS) -l boost_timer

perfor: perfor.o

.PHONY: clean count depend

clean:
	rm -f *~
	rm -f perfor perfor.o

count:
	wc -l include/*.hpp *.cc

depend: *.cc
	g++ $(CXXFLAGS) -MM *.cc > dependencies

include dependencies
