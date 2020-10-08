#
# ARGPARSER PROJECT BY: kenneth buchunju
# (C) 2020 <buchunjukenneth@gmail.com>
#

CXX=g++

CXX_FLAGS= -Wall -O2 -std=c++11

all: test

test: test.o argparser.o
	${CXX} ${CXX_FLAGS} $^ -o $@

test.o : test.cpp
	${CXX} ${CXX_FLAGS} -c $^ -o $@

argparser.o : argparser.cpp
	${CXX} ${CXX_FLAGS} -c $^ -o $@

clean: 
	rm -f test.o argparser.o