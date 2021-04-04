# Flags *******************************
# -g	debug
# -Wall	warnings
CFLAGS = -g -Wall -I.

# Libraries****************************
# -lm 	math library
LDFLAGS= -lm

# Compiler
CC=gcc

# Dependencies
IDIR =include
_DEPS = hello.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

# Objects
ODIR=obj
_OBJ = main.o hello.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# Rules for compilation
$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(ODIR)/*.o main