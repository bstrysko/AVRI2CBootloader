LNAME = AVRI2CBootloader

# Source Files
SRC = \
	src/I2CDeviceBootable.cpp

OBJ = $(SRC:.cpp=.o)
LIBNAME = lib$(LNAME).so
OUT = $(LIBNAME).1
REALNAME = $(OUT)

# Include Directories
INCLUDES = -I./include/

CCFLAGS = -g -fPIC
CCC = g++

# Library Paths
LIBS = -l I2CMaster -l IntelHexParser

# compile flags
LDFLAGS = -g
 
.SUFFIXES: .cpp
 
all: build

build: $(OUT)
 
$(OUT): $(OBJ)
	$(CCC) -shared -Wl,-soname,$(OUT) -o $(REALNAME) $(OBJ) 

.cpp.o:
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@ 

install: build
	mkdir -p /usr/include/$(LNAME)/
	cp include/* /usr/include/$(LNAME)/
	cp $(REALNAME) /usr/lib/
	ln -sf /usr/lib/$(REALNAME) /usr/lib/$(LIBNAME)

clean:
	rm -f $(OBJ) $(OUT)
