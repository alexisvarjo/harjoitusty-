# =========================
# Detect OS
# =========================
UNAME_S := $(shell uname -s)

# =========================
# Compiler and Flags
# =========================
CXX      = g++
CXXFLAGS = -std=c++17
LDFLAGS  =

# Extra flags for Google Test (change if GTest is installed differently)
GTEST_INCLUDES =
GTEST_LIBS     = -lgtest -lgtest_main

# =========================
# Platform-specific config
# =========================
ifeq ($(UNAME_S),Darwin)  # macOS
  # ICU
  CXXFLAGS += -I/opt/homebrew/opt/icu4c/include
  LDFLAGS  += -L/opt/homebrew/opt/icu4c/lib -licuuc -licudata

  # GTest typically installed via Homebrew
  # The default `-lgtest -lgtest_main` often works if your system sees them.
  # If you installed GTest at /opt/homebrew/, then also add:
  GTEST_INCLUDES += -I/opt/homebrew/include
  LDFLAGS        += -L/opt/homebrew/lib
endif

ifeq ($(UNAME_S),Linux)
  # If ICU is installed system-wide with pkg-config:
  CXXFLAGS += $(shell pkg-config --cflags icu-uc)
  LDFLAGS  += $(shell pkg-config --libs icu-uc)

  # Similarly for GTest on Linux, often installed system-wide.
  # If not, add your own includes/lib paths:
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
# List each test .cpp in TESTDIR along with the src files it needs:
TEST_TARGETS = testhuffman testlz78 testutils

# We'll build them into build/ (e.g., build/testhuffman, build/testlz78, etc.)
# Each test links specific source files from src/ that it needs.

# For clarity, define the test sources and the main .cpp:
# (Adjust if your tests rely on more or fewer src files.)
TEST_HUFFMAN_SRC = $(TESTDIR)/test_huffman.cpp $(SRCDIR)/huffman.cpp $(SRCDIR)/utils.cpp
TEST_LZ78_SRC    = $(TESTDIR)/test_lz78.cpp $(SRCDIR)/LZ78.cpp     $(SRCDIR)/utils.cpp
TEST_UTILS_SRC   = $(TESTDIR)/test_utils.cpp  $(SRCDIR)/utils.cpp

# =========================
# Default target: build the main application
# =========================
all: $(BUILDDIR)/$(APP_TARGET)

# Link the main application
$(BUILDDIR)/$(APP_TARGET): $(APP_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@
	rm -f $(APP_OBJECTS)

# Compile each .cpp into a .o in build/
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# =========================
# TEST TARGETS
# =========================

# "make test" will build all test executables AND run them in sequence
test: \
  $(BUILDDIR)/testhuffman \
  $(BUILDDIR)/testlz78    \
  $(BUILDDIR)/testutils
	@echo "========== Running testhuffman =========="
	@./$(BUILDDIR)/testhuffman
	@echo "========== Running testlz78 =========="
	@./$(BUILDDIR)/testlz78
	@echo "========== Running testutils =========="
	@./$(BUILDDIR)/testutils

# Build each test executable
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
