# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = routecipher
SOURCES = main.cpp routecipher.cpp
HEADERS = routecipher.h
OBJECTS = $(SOURCES:.cpp=.o)

# Правило по умолчанию
all: $(TARGET)

# Сборка основной программы
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Компиляция объектных файлов
main.o: main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

routecipher.o: routecipher.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c routecipher.cpp -o routecipher.o

# Очистка
clean:
	rm -f $(TARGET) $(OBJECTS)

# Пересборка
rebuild: clean all

# Запуск программы
run: $(TARGET)
	./$(TARGET)

# Отладочная сборка
debug: CXXFLAGS += -g -DDEBUG
debug: rebuild

# Проверка синтаксиса
check:
	$(CXX) $(CXXFLAGS) -fsyntax-only $(SOURCES)

# Создание архива проекта
dist: clean
	tar -czf routecipher.tar.gz *.cpp *.h Makefile README.md

.PHONY: all clean rebuild run debug check dist
