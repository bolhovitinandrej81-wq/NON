# Компилятор и флаги
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11

# Имена файлов
SOURCES = main.cpp TableRouteCipher.cpp
HEADERS = TableRouteCipher.h
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = crypto_system

# Основная цель
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Компиляция main.cpp
main.o: main.cpp TableRouteCipher.h
	$(CXX) $(CXXFLAGS) -c main.cpp

# Компиляция TableRouteCipher.cpp
TableRouteCipher.o: TableRouteCipher.cpp TableRouteCipher.h
	$(CXX) $(CXXFLAGS) -c TableRouteCipher.cpp

# Очистка
clean:
	rm -f $(OBJECTS) $(TARGET)

# Пересборка
rebuild: clean $(TARGET)

# Запуск программы
run: $(TARGET)
	./$(TARGET)

# Отладочная сборка
debug: CXXFLAGS += -g
debug: rebuild

.PHONY: clean rebuild run debug
