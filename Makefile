# source, object and header files
SRC := $(wildcard src/$(BINARY)/*.cpp)
OBJ := $(SRC:src/$(BINARY)/%.cpp=obj/$(BINARY)/%.o)
HEADER := $(wildcard include/*.h)

# preprocessor flags, second one is for ubuntu
CPPFLAGS := -Iinclude -I/usr/include/jsoncpp

# compiler flags
CXXFLAGS := -O2 -Wall -Wno-deprecated-declarations

# additional linked libraries
LDLIBS := -lzmq -ljsoncpp

# linker flags
LDFLAGS := -s

all : 
	$(MAKE) BINARY=client client
	$(MAKE) BINARY=server server

$(BINARY) : $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

obj/$(BINARY)/%.o : src/$(BINARY)/%.cpp $(HEADER) | obj/$(BINARY)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

obj/$(BINARY) :
	mkdir -p $@

clean : 
	$(RM) -r obj server client *.db

.PHONY : clean all

