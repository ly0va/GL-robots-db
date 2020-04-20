# source, object and header files
SRC := $(wildcard src/$(BINARY)/*.cpp)
OBJ := $(SRC:src/$(BINARY)/%.cpp=obj/$(BINARY)/%.o)
HEADER := $(wildcard include/*.h)

# preprocessor flags
CPPFLAGS := -Iinclude
# ubuntu has headers in another directory
ifeq ($(shell uname -n),ubuntu)
	CPPFLAGS += -I/usr/include/jsoncpp
endif

# compiler flags
CXXFLAGS := -O2 -Wall -Wno-deprecated-declarations

# additional linked libraries
LDLIBS := -lzmq -ljsoncpp

# linker flags
LDFLAGS := -s

all : 
	$(RM) *.db
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

