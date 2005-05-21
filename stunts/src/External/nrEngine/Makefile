PREFIX=

CC=g++

INCLUDES= -I ../
OPT_FLAGS= -Wall -Werror -O2
CFLAGS=$(OPT_FLAGS)

LIB_NAME= libnrEngine
RANLIB= ranlib
AR= ar
MKDIR= mkdir

OBJECTS= nrEngine.o nrCKernel.o

all: libs $(LIB_NAME) real

libs:
	$(MKDIR) libs

$(OBJECTS): %.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)


libnrEngine: $(OBJECTS)
	rm -f $(LIB_NAME).a
	$(AR) cru $(LIB_NAME).a $(OBJECTS)
	$(RANLIB) $(LIB_NAME).a
	mv $(LIB_NAME).a libs/

clean: 
	rm -f core.* core $(OBJECTS)
	rm -f *~
	rm -f DEADJOE
	rm -f *.bb *.bbg *.da *.gcov *.out
	rm -f $(LIB_NAME).a

real_clean:
	make clean
	(cd 3d && make clean)
	(cd framework && make clean)
	(cd maths && make clean)
	(cd vfs && make clean)
	(cd utils && make clean)
	
real:
	(cd 3d && make)
	(cd framework && make)
	(cd maths && make)
	(cd vfs && make)
	(cd utils && make)
				
		
distclean: clean
	rm -rf ./libs
