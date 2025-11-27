CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic
TARGET = test_modAlphaCipher

all: $(TARGET)

$(TARGET): test_modAlphaCipher.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) test_modAlphaCipher.o

test_modAlphaCipher.o: test_modAlphaCipher.cpp
	$(CXX) $(CXXFLAGS) -c test_modAlphaCipher.cpp -o test_modAlphaCipher.o

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o $(TARGET)

.PHONY: all test clean
