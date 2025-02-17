# =========================
# Detect OS
# =========================
UNAME_S := $(shell uname -s)

# =========================
# Compiler and Flags
# =========================
CXX      = g++
CXXFLAGS = -std=c++17 -g  # Keep debug symbols for debugging
LDFLAGS  =

# Extra flags for Google Test (change if GTest is installed differently)
GTEST_INCLUDES =
GTEST_LIBS     = -lgtest -lgtest_main

# =========================
# Sanitizer and Debugging
# =========================
SANITIZE ?= 0  # Enable with `make SANITIZE=1`

ifeq ($(UNAME_S),Darwin)  # macOS
  # ICU
  CXXFLAGS += -I/opt/homebrew/opt/icu4c/include
  LDFLAGS  += -L/opt/homebrew/opt/icu4c/lib -licuuc -licudata

  # GTest typically installed via Homebrew
  GTEST_INCLUDES += -I/opt/homebrew/include
  LDFLAGS        += -L/opt/homebrew/lib

  # Enable ASan if requested
  ifeq ($(SANITIZE),1)
    CXXFLAGS += -fsanitize=address
    LDFLAGS  += -fsanitize=address
  endif
endif

ifeq ($(UNAME_S),Linux)
  # If ICU is installed system-wide with pkg-config:
  CXXFLAGS += $(shell pkg-config --cflags icu-uc)
  LDFLAGS  += $(shell pkg-config --libs icu-uc)

  # Similarly for GTest on Linux
  # GTEST_INCLUDES += -I/path/to/gtest/include
  # LDFLAGS        += -L/path/to/gtest/lib
endif

# Append GTest flags to the relevant lists
CXXFLAGS += $(GTEST_INCLUDES)
LDFLAGS  += $(GTEST_LIBS)

# =========================
# Directories
# =========================
SRCDIR   = src
TESTDIR  = tests
BUILDDIR = build

# =========================
# Main Application
# =========================
APP_SOURCES  = LZ78.cpp huffman.cpp utils.cpp main.cpp
APP_OBJECTS  = $(patsubst %.cpp, $(BUILDDIR)/%.o, $(APP_SOURCES))
APP_TARGET   = main

# =========================
# Tests
# =========================
TEST_TARGETS = testhuffman testlz78 testutils

TEST_HUFFMAN_SRC = $(TESTDIR)/test_huffman.cpp $(SRCDIR)/huffman.cpp $(SRCDIR)/utils.cpp
TEST_LZ78_SRC    = $(TESTDIR)/test_lz78.cpp $(SRCDIR)/LZ78.cpp     $(SRCDIR)/utils.cpp
TEST_UTILS_SRC   = $(TESTDIR)/test_utils.cpp  $(SRCDIR)/utils.cpp

# =========================
# Default target: build the main application
# =========================
all: $(BUILDDIR)/$(APP_TARGET)

$(BUILDDIR)/$(APP_TARGET): $(APP_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@
	rm -f $(APP_OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# =========================
# TEST TARGETS
# =========================
test: \
  $(BUILDDIR)/testhuffman \
  $(BUILDDIR)/testlz78    \
  $(BUILDDIR)/testutils
ifeq ($(SANITIZE),1)
  ifeq ($(UNAME_S),Linux)
	@echo "========== Running testhuffman with Valgrind =========="
	@valgrind --leak-check=full --show-leak-kinds=all ./$(BUILDDIR)/testhuffman
	@echo "========== Running testlz78 with Valgrind =========="
	@valgrind --leak-check=full --show-leak-kinds=all ./$(BUILDDIR)/testlz78
	@echo "========== Running testutils with Valgrind =========="
	@valgrind --leak-check=full --show-leak-kinds=all ./$(BUILDDIR)/testutils
  else
	@echo "========== Running testhuffman =========="
	@./$(BUILDDIR)/testhuffman
	@echo "========== Running testlz78 =========="
	@./$(BUILDDIR)/testlz78
	@echo "========== Running testutils =========="
	@./$(BUILDDIR)/testutils
  endif
else
	@echo "========== Running testhuffman =========="
	@./$(BUILDDIR)/testhuffman
	@echo "========== Running testlz78 =========="
	@./$(BUILDDIR)/testlz78
	@echo "========== Running testutils =========="
	@./$(BUILDDIR)/testutils
endif

$(BUILDDIR)/testhuffman: $(TEST_HUFFMAN_SRC)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

$(BUILDDIR)/testlz78: $(TEST_LZ78_SRC)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

$(BUILDDIR)/testutils: $(TEST_UTILS_SRC)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

# =========================
# Cleanup
# =========================
clean:
	rm -rf $(BUILDDIR)

.PHONY: all clean test
