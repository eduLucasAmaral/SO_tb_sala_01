CXX = g++
CXXFLAGS = -std=c++17 -O2 -I./src
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

all: emulador

emulador: $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o emulador

clean:
	rm -f emulador $(OBJS)
