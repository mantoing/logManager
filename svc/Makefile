INCS=-I../include
LIB_PATH=-L../lib
LIBS=-ljaynet
.c.o:
	$(CC) -c $<  $(INCS)


svr : svr.o
	$(CC) -o $@  svr.o $(LIB_PATH) $(LIBS)

svc : svc.o
	$(CC) -o $@  svc.o $(LIB_PATH) $(LIBS)
cli : cli.o
	$(CC) -o $@  cli.o $(LIB_PATH) $(LIBS)
clean :
	rm -f *.o
