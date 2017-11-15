CPPFLAGS=-std=c++14 -g -O0 -Wall -Wextra

SRCS=main.cpp
OBJS=$(SRCS:.cpp=.o)

all: stream

stream: $(OBJS)
	$(CXX) $^ -o $@

%.o: %.cpp %.h
	$(CXX) $(CPPFLAGS) -c $<

clean:
	rm -rf stream
	rm -rf $(OBJS)