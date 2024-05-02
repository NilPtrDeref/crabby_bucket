ifeq ($(WEB),)
	CXX := gcc
	CXXFLAGS := -Wall -Wextra -g -std=c++2b
	LDLIBS := -lstdc++ -lraylib -lGL -lm
	TARGET := crabby
else
	CXX := em++
	CXXFLAGS := -Os -Wall -s WASM=1 -s ASYNCIFY --preload-file assets
	LDLIBS := -lstdc++ -lwebraylib -s USE_GLFW=3 -lm
	TARGET := crabby.html
endif

INCLUDES := -I./src
LDFLAGS := -L./lib

SRCDIR := src
OBJDIR := build
CPPFILES := $(filter-out $(SRCDIR)/main.cpp,$(wildcard $(SRCDIR)/*.cpp))
OBJFILES := $(CPPFILES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Default target
all: $(OBJFILES)
	$(CXX) -o $(OBJDIR)/$(TARGET) src/main.cpp $(CXXFLAGS) $(OBJFILES) $(INCLUDES) $(LDFLAGS) $(LDLIBS)

run: all
	./$(TARGET)

# To obtain object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJDIR)/*

# Declare non-file targets
.PHONY: all clean
