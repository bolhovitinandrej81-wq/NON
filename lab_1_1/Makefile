# Компилятор и флаги
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11
TARGET = crypto_system

# Исходные файлы
SOURCES = main.cpp TableRouteCipher.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = TableRouteCipher.h

# Основная цель
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Компиляция main.cpp
main.o: main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c main.cpp

# Компиляция TableRouteCipher.cpp
TableRouteCipher.o: TableRouteCipher.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c TableRouteCipher.cpp

# Очистка
clean:
	rm -f $(OBJECTS) $(TARGET)

# Пересборка
rebuild: clean $(TARGET)

# Установка (если нужно)
install: $(TARGET)
	@echo "Программа собрана: ./$(TARGET)"

# Помощь
help:
	@echo "Доступные цели:"
	@echo "  all       - собрать программу (по умолчанию)"
	@echo "  clean     - удалить объектные файлы и программу"
	@echo "  rebuild   - пересобрать программу"
	@echo "  install   - собрать программу (аналогично all)"
	@echo "  help      - показать эту справку"

# Псевдонимы
all: $(TARGET)

.PHONY: all clean rebuild install help
