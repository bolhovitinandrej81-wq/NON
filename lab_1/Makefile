# Компилятор и флаги
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2
LDFLAGS = 

# Целевые файлы
TARGET = cipher
SOURCES = main.cpp modAlphaCipher.cpp
HEADERS = modAlphaCipher.h
OBJECTS = $(SOURCES:.cpp=.o)

# Правило по умолчанию
all: $(TARGET)

# Сборка исполняемого файла
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Компиляция main.cpp
main.o: main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

# Компиляция modAlphaCipher.cpp
modAlphaCipher.o: modAlphaCipher.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c modAlphaCipher.cpp -o modAlphaCipher.o

# Очистка
clean:
	rm -f $(OBJECTS) $(TARGET)

# Пересборка
rebuild: clean all

# Запуск программы
run: $(TARGET)
	./$(TARGET)

# Отладочная сборка
debug: CXXFLAGS += -g -DDEBUG
debug: rebuild

# Установка (если нужно)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Список целей
.PHONY: all clean rebuild run debug install
