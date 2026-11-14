CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2 -g -fsanitize=address -O1

TARGET := kingdoms
BUILD  := build
DEBUG := kingdoms.dSYM
LOG := output.log

SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:%.cpp=$(BUILD)/%.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf $(BUILD) $(DEBUG) $(TARGET) $(LOG)
	clear

-include $(DEPS)
