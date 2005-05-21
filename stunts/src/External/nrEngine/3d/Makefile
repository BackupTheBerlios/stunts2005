PREFIX=

CC=g++

INCLUDES= -I ../../
OPT_FLAGS= -Wall -Werror -O2
CFLAGS=$(OPT_FLAGS)
 
LIB_NAME= libnr3DEngine
RANLIB= ranlib
AR= ar

OBJECTS= nr3DFuncs.o stacks.o nrCTexture.o nrCFont.o nrCTextureLoader.o\
		nrCCamera.o nrCFrustum.o nrPBuffer.o nrCLensFlare.o\
		nrCLight.o nrCPointLight.o nrCSpotLight.o nrCDirectionLight.o
	
all: $(LIB_NAME)
	
$(OBJECTS): %.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)
		

libnr3DEngine: $(OBJECTS)
	rm -f $(LIB_NAME).a
	$(AR) cru $(LIB_NAME).a $(OBJECTS)
	$(RANLIB) $(LIB_NAME).a
	mv $(LIB_NAME).a ../libs/
	
clean: 
	rm -f core.* core $(OBJECTS)
	rm -f *~
	rm -f DEADJOE
	rm -f *.bb *.bbg *.da *.gcov *.out
	rm -f $(LIB_NAME).a
	
		
