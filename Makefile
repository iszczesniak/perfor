TARGETS = perfor
TARGET_OBJS = $(addsuffix .o, $(TARGETS))

OBJS = args.o progress.o

CXXFLAGS = -g
#CXXFLAGS = -O3

CXXFLAGS := $(CXXFLAGS) -Wall -std=c++11 -Wno-deprecated

# Boost
#BOOSTPATH := /usr/local/boost-1.43
#CXXFLAGS := $(CXXFLAGS) -I $(BOOSTPATH)/include
#LDFLAGS := $(LDFLAGS) -L $(BOOSTPATH)/lib
LDFLAGS := $(LDFLAGS) -l boost_program_options-mt
LDFLAGS := $(LDFLAGS) -l boost_system-mt
LDFLAGS := $(LDFLAGS) -l boost_thread-mt
LDFLAGS := $(LDFLAGS) -l boost_timer-mt

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
