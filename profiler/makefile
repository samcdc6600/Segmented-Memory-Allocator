CC      = clang
CFLAGS	= -Wall -std=c11 -Wpedantic -Wextra -O3
#LIBS	= -l/include/libMM
LIBS	= -llibMM.a
BIN_NAME = profiler
SRC   = ./src
IDIR  = $(SRC)/include
ODIR  = ./obj
DEPS  = $(wildcard $(IDIR)/*.h)
C_DIRS  =  $(wildcard $(SRC)/*.c) # Get list of .c file directories.
C_FILES = $(subst $(SRC)/,,$(C_DIRS)) # Strip srings of paths.
OBJ_FILES = $(patsubst %.c,%.o,$(C_FILES)) # Replace .c with .o.
OBJ       = $(patsubst %,$(ODIR)/%,$(OBJ_FILES))  # Prepend object directory to .o file names.


$(ODIR)/%.o: $(SRC)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN_NAME): $(OBJ)
	$(CC) -v -o $@ $^ $(LIBS)

.PHONY: clean

# Files with '#' and '~' seem to be left around be emacs >:'^(
clean:
	$(RM) $(BIN_NAME) $(ODIR)/*.o *.core *~ $(SRC)/*~ $(IDIR)/*~ \#*\# $(SRC)/\#*\# $(IDIR)/\#*\#
