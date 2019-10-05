#CXX      = clang++
CXX	 = g++
AR	 = ar
AR_OPS	 = rcs
#CXXFLAGS = -std=c++14 -Wall -Wpedantic -Wextra -O3
CXXFLAGS = -std=c++11 -Wall -Wpedantic -Wextra -O3
LIB_NAME = libMM.a
SRC   = ./src
IDIR  = $(SRC)/include
ODIR  = ./obj
DEPS  = $(wildcard $(IDIR)/*.hpp)
CPP_DIRS  =  $(wildcard $(SRC)/*.cpp) # Get list of .cpp file directories.
CPP_FILES = $(subst $(SRC)/,,$(CPP_DIRS)) # Strip srings of paths.
OBJ_FILES = $(patsubst %.cpp,%.o,$(CPP_FILES)) # Replace .cpp with .o.
# Prepend object directory to .o file names.
OBJ       = $(patsubst %,$(ODIR)/%,$(OBJ_FILES))

# The following things that are commented out dont work for some unknown reason.

# Create multi line variable (CLEAN_TARGET) for "clean" target (we don't want to
# break the rules and go over 80 lines :'( .) 
#define CLEAN_TARGET
#$(RM) $(LIB_NAME) $(ODIR)/*.o *.core *~ $(SRC)/*~ $(IDIR)/*~ \#*\# $(SRC)/\#*\#
#$(IDIR)/\#*\#
#endef
#export CLEAN_TARGET # Export as an environment veriable to the shell.


$(ODIR)/%.o: $(SRC)/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(LIB_NAME): $(OBJ)
	$(AR) $(AR_OPS) $@ $^

.PHONY: clean

# Files with '#' and '~' seem to be left around be emacs >:'^(
clean:
	$(RM) $(LIB_NAME) $(ODIR)/*.o *.core *~ $(SRC)/*~ $(IDIR)/*~ \#*\# $(SRC)/\#*\# $(IDIR)/\#*\#
