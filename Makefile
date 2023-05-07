CC = gcc
SVOBJ = server.o coup.o msg.o host.o
CLOBJ = client.o coup.o handler.o

server: $(SVOBJ)
	$(CC) $(INCDIR) $(LIBDIR) -o $(@) $(SVOBJ) $(LIB) $(FLAG) -g -O0

client: $(CLOBJ)
	$(CC) $(INCDIR) $(LIBDIR) -o $(@) $(CLOBJ) $(LIB) $(FLAG) -g -O0

.cpp.o:
	$(CC) $(INCDIR) $(LIBDIR) -c -g -O0 $<

clean:
	rm -rf *.o
	rm -rf test server client
