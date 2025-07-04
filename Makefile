# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g
LDLIBS = -lglpk

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj

# GLPK linking (if needed for custom installations)
PKG_CONFIG_PATH ?= /usr/lib/x86_64-linux-gnu/pkgconfig

# Source files
SOURCES = main.cpp \
          $(SRCDIR)/entropy_utils.cpp \
          $(SRCDIR)/inequality_utils.cpp \
          $(SRCDIR)/shannon_generator.cpp \
          $(SRCDIR)/parser.cpp \
          $(SRCDIR)/glpk_solver.cpp

# Object files (generated from source files)
OBJECTS = $(SOURCES:%.cpp=$(OBJDIR)/%.o)

# Include directories
INCLUDES = -I$(INCDIR)

# Target executable
TARGET = itip

# Default target
all: $(TARGET)

# Create object directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR) $(OBJDIR)/$(SRCDIR)

# Rule to build the target
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

# Rule to build object files
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean target
clean:
	rm -rf $(TARGET) $(OBJDIR)

# Debug target (with debug symbols, no optimization)
debug: CXXFLAGS = -std=c++17 -Wall -Wextra -g -DDEBUG
debug: $(TARGET)

# Release target (optimized, no debug symbols)
release: CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -DNDEBUG
release: clean $(TARGET)

# Install target (customize paths as needed)
install: $(TARGET)
	install -d $(DESTDIR)/usr/local/bin
	install -m 755 $(TARGET) $(DESTDIR)/usr/local/bin

# Uninstall target
uninstall:
	rm -f $(DESTDIR)/usr/local/bin/$(TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  debug    - Build with debug symbols"
	@echo "  release  - Build optimized release version"
	@echo "  clean    - Remove build artifacts"
	@echo "  install  - Install the binary"
	@echo "  uninstall- Remove installed binary"
	@echo "  help     - Show this help message"

# Dependencies (auto-generated)
-include $(OBJECTS:.o=.d)

# Generate dependency files
$(OBJDIR)/%.d: %.cpp | $(OBJDIR)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -MM -MT '$(OBJDIR)/$*.o' $< > $@

# Phony targets
.PHONY: all clean debug release install uninstall help