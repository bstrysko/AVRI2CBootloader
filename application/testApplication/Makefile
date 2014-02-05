LINK_TARGET = test

OBJS =  \
	main.o \
	src/TestDevice.o

CFLAGS = -Iinclude/ -I/usr/include/
LIBS = -l I2CMaster -l AVRI2CBootloader -l IntelHexParser

REBUILDABLES = $(OBJS) $(LINK_TARGET)

all : $(LINK_TARGET)
	@echo "Build complete"

clean :
	rm -f $(REBUILDABLES)
	@echo "Clean complete"

$(LINK_TARGET) : $(OBJS)
	g++ -g -o $@ $^ $(LIBS)

%.o : %.cpp
	g++ $(CFLAGS) -g -o $@ -c $< $(LIBS)
