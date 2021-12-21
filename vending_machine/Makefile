TARGET=vendingmachine
TEST=$(TARGET)_test
CROSS=$(TARGET)_cross

INCLUDEFLAGS=-Isrc -Iinclude -Iinclude/interfaces 
CXXFLAGS=-Wall -Werror -Wextra -pedantic -ggdb -O0 -lpthread -g $(INCLUDEFLAGS)
TESTFLAGS=-lgtest -pthread
CXX=g++
CC_LPC = /usr/local/xtools/arm-unknown-linux-uclibcgnueabi/bin/arm-linux-g++
#LD_LPC = /usr/local/xtools/arm-unknown-linux-uclibcgnueabi/bin/arm-unknown-linux-uclibcgnueabi-ld
#LDFLAGS = -lpthread -m32 -static -nostdlib

SOURCES=$(wildcard src/*.cpp)
HEADERS=$(wildcard include/*.h) \
        $(wildcard include/interfaces/*.h)

TEST_SOURCES=$(filter-out src/main.cpp, $(SOURCES)) \
			$(wildcard test/*.cpp)

TEST_HEADERS=$(HEADERS) \
			 $(wildcard test/*.h)

.PHONY: all clean test

all: $(CROSS)

$(TARGET): $(SOURCES) $(HEADERS) Makefile
	@$(CXX) $(CXXFLAGS)  -std=c++11 -o $@ $(SOURCES)

$(CROSS): $(SOURCES) $(HEADERS) Makefile
	@$(CC_LPC) $(CXXFLAGS) -std=c++0x -o $@ $(SOURCES)

$(TEST): $(TEST_SOURCES) $(TEST_HEADERS) Makefile
	@$(CXX) $(CXXFLAGS) -Itest $(TEST_SOURCES) -o $@ $(TESTFLAGS)

clean:
	@rm $(TARGET) $(TEST) $(CROSS)

test: $(TEST)
	@./$(TEST) 2> /dev/null
