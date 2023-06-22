CXX ?= g++
CXXFLAGS ?= -O2

CXXFLAGS := ${CXXFLAGS} -pthread

all:
	${CXX} src/*.cpp -o vigenere ${CXXFLAGS}
clean:
	rm -f program