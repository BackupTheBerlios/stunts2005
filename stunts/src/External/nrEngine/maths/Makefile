PREFIX=

CC=g++

INCLUDES= -I ../../
OPT_FLAGS= -Wall -Werror -O2
CFLAGS=$(OPT_FLAGS)
 
LIB_NAME= libnrMath
RANLIB= ranlib
AR= ar

OBJECTS= nrCSpline.o 

all: $(LIB_NAME)
	
$(OBJECTS): %.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)
		

libnrMath: $(OBJECTS)
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
	
		
