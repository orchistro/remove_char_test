.PHONY: all clean

REL=$(filter release,$(MAKECMDGOALS))
ifeq ($(REL), release)
export OPTIMIZE_LEVE = 2
else
export OPTIMIZE_LEVE = 0
endif

release debug verbose: all

all: foo

SRCS := $(shell find . -name '*.cpp')
OBJS := $(SRCS:.cpp=.o)

foo: $(OBJS)
	gcc -o $@ $^ -lstdc++ -Wall

%.o: %.cpp
	gcc -std=c++17 -I../../index.1/externals/libboost/include -c -g -O$(OPTIMIZE_LEVE) -Wall -o $@ $<

clean:
	rm -rf foo *.o core*

