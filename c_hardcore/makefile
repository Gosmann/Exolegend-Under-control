CC=gcc
CFLAGS=-I.
DEPS = aap.h
OBJ = main.o aap.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

output: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -r *.o