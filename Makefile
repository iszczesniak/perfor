# Use the C++ linker.
LINK.o = $(LINK.cc)

# Compiler flags.
CXXFLAGS := -g -Wall -std=c++11 -Wno-deprecated -I include

# Linker flags: use Boost.
LDFLAGS := $(LDFLAGS) -l boost_system
LDFLAGS := $(LDFLAGS) -l boost_thread
LDFLAGS := $(LDFLAGS) -l boost_timer

perfor: perfor.o

.PHONY: clean count

clean:
	rm -f *~
	rm -f perfor perfor.o

count:
	wc -l include/*.hpp *.cc

depend: *.cc
	g++ $(CXXFLAGS) -MM *.cc > depend

include depend
