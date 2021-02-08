
SRC_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
BUILD_DIR = $(SRC_DIR)build
CC = gcc
CXX = g++
AR = ar
SHARED_LIB := libjsoncpp.so
STATIC_LIB := libjsoncpp.a

SOURCE = src/value.cpp \
		 src/error.cpp \
		 src/utf8.cpp \
		 src/json.cpp

OBJ_FILES := $(SOURCE:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJ_FILES:.o=.d)
CPPFLAGS := -std=c++11 -I$(SRC_DIR)inc -g -Wall -Werror -MMD -MP

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: all static shared install uninstall test clean

all: static shared

static: $(OBJ_FILES)
	$(AR) rcs $(BUILD_DIR)/$(STATIC_LIB) $^

shared: $(OBJ_FILES)
	$(CXX) $^ -shared -o $(BUILD_DIR)/$(SHARED_LIB)

test: static
	make -f Makefile.test $@

clean:
	rm -fr $(BUILD_DIR)
