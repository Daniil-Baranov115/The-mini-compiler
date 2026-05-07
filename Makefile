CXX = g++
CXXFLAGS = -std=c++17 -Wall -I src -I /usr/lib/llvm-18/include
LDFLAGS = -L /usr/lib/llvm-18/lib -lLLVM-18

SRCS = src/main.cpp src/driver.cpp src/ast.cpp src/sema.cpp src/llvm_codegen.cpp src/lexer.cpp src/parser.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = mini_compiler

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
