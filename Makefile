CXX ?= g++
CXXFLAGS ?= -O2

CXXFLAGS := ${CXXFLAGS} -pthread

all:
	${CXX} src/*.cpp -o program ${CXXFLAGS}
clean:
	rm -f program