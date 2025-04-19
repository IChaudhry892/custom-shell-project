CC=gcc
CFLAGS=-Wall -g
DEPS = newshell.h builtins.h
OBJ = main.o newshell.o builtins.o

all: newshell

# Build the .o files from .c files; $@ -> target, $< -> .c files
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

# Make the executable; $^ -> .o files
newshell: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o newshell
