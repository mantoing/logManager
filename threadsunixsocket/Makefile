INCS=-I../include
LIB_PATH=-L../lib
LIBS=-ljaynet
.c.o:
	$(CC) -c $<  $(INCS)

unix_cli : unix_cli.o
	$(CC) -o $@  unix_cli.o $(LIB_PATH) $(LIBS)

unix_main : unix_main.o
	$(CC) -o $@  unix_main.o $(LIB_PATH) $(LIBS)
unix_sub : unix_sub.o
	$(CC) -o $@  unix_sub.o $(LIB_PATH) $(LIBS)
clean :
	rm -f *.o
