LINK_TARGET = i2cAVRFlash

OBJS =  \
	main.o \
	src/I2CDeviceBootable.o \
	src/I2CDeviceBootLoader.o \
	src/ApplicationPage.o \
	src/ApplicationProgram.o

DEPFILES = $(patsubst %.o,%.dep,$(OBJS))

CFLAGS = -Iinclude/
LIBS = -l I2CMaster

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
