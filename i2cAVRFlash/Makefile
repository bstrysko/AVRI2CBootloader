LINK_TARGET = i2cAVRFlash

OBJS =  \
	main.o

CFLAGS = -Iinclude/ -I/usr/include/
LIBS = -l I2CMaster -l IntelHexParser -l AVRI2CBootloader

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

install:
	cp $(LINK_TARGET) /usr/bin/
