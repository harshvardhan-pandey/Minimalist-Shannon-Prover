# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

# GLPK linking
LDLIBS = -lglpk
PKG_CONFIG_PATH ?= /usr/lib/x86_64-linux-gnu/pkgconfig

# Source and output
SRC = src/main.cpp src/entropy_utils.cpp src/shannon_generator.cpp
INC = -Iinclude
TARGET = itip

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(TARGET)
