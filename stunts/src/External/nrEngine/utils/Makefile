PREFIX=

CC=g++

INCLUDES= -I ../../
OPT_FLAGS= -Wall -Werror -O2
CFLAGS=$(OPT_FLAGS)
 
LIB_NAME= libnrUtils
RANLIB= ranlib
AR= ar

OBJECTS= nrCClock.o nrCTimer.o nrCTimeSourceFramework.o nrIResource.o nrCResourcePtr.o\
	nrCResourceManager.o Resources.o\
	nrCLog.o nrFuncs.o nrSettings.o\
	console/nrCConsoleBase.o console/nr_console_default_functions.o\
	console/nrCConsole.o
	
all: $(LIB_NAME)
	
$(OBJECTS): %.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)
		

libnrUtils: $(OBJECTS)
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
	
		
