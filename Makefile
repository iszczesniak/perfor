TARGETS = main
TARGET_OBJS = $(addsuffix .o, $(TARGETS))
LINK.o = $(LINK.cc)

OBJS = args.o progress.o

CXXFLAGS = -g
#CXXFLAGS = -O3

CXXFLAGS := $(CXXFLAGS) -Wall -std=c++11 -Wno-deprecated

# Boost
LDFLAGS := $(LDFLAGS) -l boost_program_options
LDFLAGS := $(LDFLAGS) -l boost_system
LDFLAGS := $(LDFLAGS) -l boost_thread
LDFLAGS := $(LDFLAGS) -l boost_timer

all: $(TARGETS)

availa: $(OBJS)

.PHONY: clean count depend test

clean:
	rm -f *~
	rm -f $(OBJS)
	rm -f $(TARGET_OBJS)
	rm -f $(TARGETS)

count:
	wc -l *.hpp *.cc

depend:
	g++ -std=c++11 -MM *.cc > dependencies

test: $(OBJS)
	cd test; make

include dependencies
