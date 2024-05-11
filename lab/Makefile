INCS=-I../include
LIB_PATH=-L../lib
LIBS=-ljaynet
.c.o:
	$(CC) -c $<  $(INCS)


httpclijaeywon : httpclijaeywon.o
	$(CC) -o $@  httpclijaeywon.o $(LIB_PATH) $(LIBS)

clean :
	rm -f *.o
