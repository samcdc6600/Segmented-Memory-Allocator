CC      = clang
#CC	= gcc
CFLAGS	= -Wall -std=c11 -Wpedantic -Wextra -O3
# -lstdc++ is very very important here!
# Gcc and clang don't link it by default like g++ and clang++!
# This took a while to figour out. Not as long as the fact that
# I accidently wasn't linking one of the .o files in the other
# make file when building the library causing the linker not to
# be able to fine setAllocationAlgorithm, alloc and dealloc
# (that took ages to figour out >:'( haha.
LIBS	= src/include/libMM.a -lstdc++ -lpthread -lrt
BIN_NAME = profiler
SRC   = ./src
IDIR  = $(SRC)/include
ODIR  = ./obj
DEPS  = $(wildcard $(IDIR)/*.h)
C_DIRS  =  $(wildcard $(SRC)/*.c) # Get list of .c file directories.
C_FILES = $(subst $(SRC)/,,$(C_DIRS)) # Strip srings of paths.
OBJ_FILES = $(patsubst %.c,%.o,$(C_FILES)) # Replace .c with .o.
# Prepend object directory to .o file names.
OBJ       = $(patsubst %,$(ODIR)/%,$(OBJ_FILES))


# The things commented out below seem to work. You can see in the terminal that
#they are supposedly being executed. However non of the file were actually
#removed >:'(. I tried.

# Create multi line variable (CLEAN_TARGET) for "clean" target (we don't want to
# break the rules and go over 80 lines (although we did by one line in the build
# script :'( .)
#define CLEAN_TARGET
#$(RM) $(BIN_NAME) $(ODIR)/*.o *.core *~ $(SRC)/*~ $(IDIR)/*~ \#*\# $(SRC)/\#*\#
#$(RM) $(IDIR)/\#*\#
#endef
#export CLEAN_TARGET # Export as an environment veriable to the shell.



$(ODIR)/%.o: $(SRC)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN_NAME): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

.PHONY: clean

# Files with '#' and '~' seem to be left around be emacs >:'^(
clean:
	$(RM) $(BIN_NAME) $(ODIR)/*.o *.core *~ $(SRC)/*~ $(IDIR)/*~ \#*\# $(SRC)/\#*\# $(RM) $(IDIR)/\#*\#
#	@echo "$$CLEAN_TARGET"
