# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
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

# Запуск тестов
test: $(TARGET)
	@echo "=== Тестирование шифра ==="
	@echo "Тест 1: Пустой ключ"
	@echo -e "\\n" | ./$(TARGET) 2>&1 | grep "Ошибка" || true
	@echo "Тест 2: Ключ с недопустимыми символами"
	@echo -e "abc123\\n" | ./$(TARGET) 2>&1 | grep "Ошибка" || true
	@echo "Тест 3: Нормальная работа"
	@echo -e "КЛЮЧ\\n1\\nТЕКСТ\\n2\\nХЦМФУ\\n0" | ./$(TARGET) 2>&1 | grep -E "(Зашифрованный|Расшифрованный)" || true

# Установка (если нужно)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Помощь
help:
	@echo "Доступные цели:"
	@echo "  all      - сборка проекта (по умолчанию)"
	@echo "  clean    - удаление скомпилированных файлов"
	@echo "  rebuild  - пересборка проекта"
	@echo "  test     - запуск базовых тестов"
	@echo "  install  - установка в систему"

.PHONY: all clean rebuild test install help
