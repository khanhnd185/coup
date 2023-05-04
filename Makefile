#HOME = 
#INCDIR = -I. -I$(HOME)/include
#LIBDIR = -L. -L$(HOME)/lib-linux64
#LIB = -lm
#FLAG = -Wl,-rpath,$(HOME)/lib-linux64/
CC = gcc
OBJ = test.o coup.o interface.o host.o
SVOBJ = server.o
CLOBJ = client.o

test: $(OBJ)
	$(CC) $(INCDIR) $(LIBDIR) -o $(@) $(OBJ) $(LIB) $(FLAG) -g -O0

server: $(SVOBJ)
	$(CC) $(INCDIR) $(LIBDIR) -o $(@) $(SVOBJ) $(LIB) $(FLAG) -g -O0

client: $(CLOBJ)
	$(CC) $(INCDIR) $(LIBDIR) -o $(@) $(CLOBJ) $(LIB) $(FLAG) -g -O0

.cpp.o:
	$(CC) $(INCDIR) $(LIBDIR) -c -g -O0 $<

clean:
	rm -rf *.o
	rm -rf test server client