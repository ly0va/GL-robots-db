# source, object and header files
SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:src/%.cpp=obj/%.o)
HEADER := $(wildcard include/*.h)

# preprocessor flags
CPPFLAGS := -Iinclude

# compiler flags
CXXFLAGS := -Wall -O2

# additional linked libraries
# LDLIBS := -lzmq -ljsoncpp

# linker flags
LDFLAGS := -s

all : server

server : $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

obj/%.o : src/%.cpp $(HEADER) | obj
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

obj :
	mkdir $@

clean : 
	$(RM) -r obj main

.PHONY : clean all

