CXX := gcc
CXXFLAGS := -Wall -Wextra -g -std=c++2b
INCLUDES := -I./src
LDFLAGS := -L./lib
LDLIBS := -lstdc++ -lraylib -lGL -lm
TARGET := crabby

SRCDIR := src
OBJDIR := build
CPPFILES := $(filter-out $(SRCDIR)/main.cpp,$(wildcard $(SRCDIR)/*.cpp))
OBJFILES := $(CPPFILES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Default target
all: $(OBJFILES)
	$(CXX) $(CXXFLAGS) $(OBJFILES) src/main.cpp $(INCLUDES) $(LDFLAGS) $(LDLIBS) -o $(TARGET)

run: all
	./$(TARGET)

# To obtain object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJFILES) $(TARGET)

# Declare non-file targets
.PHONY: all clean
