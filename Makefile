CXX := gcc
CXXFLAGS := -Wall -Wextra -std=c++2b -g
INCLUDES := -I./src -I/usr/include/SDL2
LDFLAGS := -L/usr/lib
LDLIBS := -lstdc++ -lSDL2 -lSDL2_image -lSDL2_ttf
TARGET := crabby

SRCDIR := src
OBJDIR := build
CPPFILES := $(filter-out $(SRCDIR)/main.cpp,$(wildcard $(SRCDIR)/*.cpp))
OBJFILES := $(CPPFILES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Default target
all: $(OBJFILES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS) $(OBJFILES) src/main.cpp -o $(TARGET)

# To obtain object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) $(LDLIBS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJFILES) $(TARGET)

# Declare non-file targets
.PHONY: all clean
