LINK_TARGET = i2cAVRFlash

OBJS =  \
	main.o \
	src/I2CDeviceBootable.o

DEPFILES = $(patsubst %.o,%.dep,$(OBJS))

CFLAGS = -Iinclude/ -I/usr/include/
LIBS = -l I2CMaster -l IntelHexParser

REBUILDABLES = $(OBJS) $(LINK_TARGET) $(DEPFILES)

all : $(LINK_TARGET)
	@echo "Build complete"

clean :
	rm -f $(REBUILDABLES)
	@echo "Clean complete"

$(LINK_TARGET) : $(OBJS)
	g++ -g -o $@ $^ $(LIBS)

%.o : %.cpp
	g++ $(CFLAGS) -g -o $@ -c $< $(LIBS)

%.dep : %.cpp
	g++ -M $(CFLAGS) $< > $@ $(LIBS)

include $(OBJS:.o=.dep)
