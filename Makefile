# Makefile

# Detect OS
UNAME_S := $(shell uname -s)

# Default flags
CXX      = g++
CXXFLAGS = -std=c++17
LDFLAGS  =

# Platform-specific flags
ifeq ($(UNAME_S),Darwin)  # macOS
  CXXFLAGS += -I/opt/homebrew/opt/icu4c/include
  LDFLAGS  += -L/opt/homebrew/opt/icu4c/lib -licuuc -licudata
endif

ifeq ($(UNAME_S),Linux)  # Linux
  # If ICU is installed system-wide, pkg-config is the easiest:
  #    sudo apt-get install libicu-dev  (Debian/Ubuntu)
  #    or the equivalent on your distro
  # Then rely on pkg-config to find the right cflags and libs:
  CXXFLAGS += $(shell pkg-config --cflags icu-uc)
  LDFLAGS  += $(shell pkg-config --libs icu-uc)

  # If you don't want to rely on pkg-config, or your ICU is installed
  # in a non-standard location, specify them manually, e.g.:
  # CXXFLAGS += -I/usr/include
  # LDFLAGS  += -L/usr/lib -licuuc -licudata
endif

# Directories
SRCDIR   = src
BUILDDIR = build

# Source files
SOURCES  = LZ78.cpp huffman.cpp utils.cpp main.cpp
OBJECTS  = $(patsubst %.cpp, $(BUILDDIR)/%.o, $(SOURCES))
TARGET   = main

all: $(BUILDDIR)/$(TARGET)

$(BUILDDIR)/$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LDFLAGS) -o $@
	rm -f $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)

.PHONY: all clean
