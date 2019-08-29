CXX      = clang++
CXXFLAGS = -std=c++14 -Wall -Wpedantic -Wextra -O3
BIN_NAME = mm
SRC   = ./src
IDIR  = $(SRC)/include
ODIR  = ./obj
DEPS  = $(wildcard $(IDIR)/*.hpp)
CPP_DIRS  =  $(wildcard $(SRC)/*.cpp) # Get list of .cpp file directories.
CPP_FILES = $(subst $(SRC)/,,$(CPP_DIRS)) # Strip srings of paths.
OBJ_FILES = $(patsubst %.cpp,%.o,$(CPP_FILES)) # Replace .cpp with .o.
OBJ       = $(patsubst %,$(ODIR)/%,$(OBJ_FILES))  # Prepend object directory to .o file names.


$(ODIR)/%.o: $(SRC)/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(BIN_NAME): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean

# Files with '#' and '~' seem to be left around be emacs >:'^(
clean:
	$(RM) $(BIN_NAME) $(ODIR)/*.o *.core *~ $(SRC)/*~ $(IDIR)/*~ \#*\# $(SRC)/\#*\# $(IDIR)/\#*\#
