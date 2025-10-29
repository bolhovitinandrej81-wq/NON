# Компилятор и флаги
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g
TARGET = crypto_system

# Исходные файлы
SOURCES = main.cpp TableRouteCipher.cpp
HEADERS = TableRouteCipher.h
OBJECTS = $(SOURCES:.cpp=.o)

# Основная цель
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Компиляция объектных файлов
main.o: main.cpp TableRouteCipher.h
	$(CXX) $(CXXFLAGS) -c main.cpp

TableRouteCipher.o: TableRouteCipher.cpp TableRouteCipher.h
	$(CXX) $(CXXFLAGS) -c TableRouteCipher.cpp

# Очистка
clean:
	rm -f $(TARGET) $(OBJECTS)

# Перекомпиляция
rebuild: clean $(TARGET)

# Запуск программы
run: $(TARGET)
	./$(TARGET)

# Тестирование исключений
test: $(TARGET)
	@echo "=== Тестирование исключений ==="
	@echo "1. Тест пустого текста после пробелов:"
	@echo "    Ввод: текст='    ', ключ=3"
	@echo "    Ожидается: 'Отсутствует открытый текст'"
	@echo ""
	@echo "2. Тест некорректных символов:"
	@echo "    Ввод: текст='hello123', ключ=3"
	@echo "    Ожидается: 'Некорректные символы в строке'"
	@echo ""
	@echo "3. Тест маленького ключа:"
	@echo "    Ввод: текст='hello', ключ=1"
	@echo "    Ожидается: 'Некорректный размер ключа'"
	@echo ""
	@echo "4. Тест большого ключа:"
	@echo "    Ввод: текст='hi', ключ=5"
	@echo "    Ожидается: 'Некорректный размер ключа'"
	@echo ""
	@echo "5. Тест корректной работы:"
	@echo "    Ввод: текст='Hello World', ключ=3"
	@echo "    Ожидается: Успешное шифрование/дешифрование"

# Отладочная информация
debug:
	@echo "Sources: $(SOURCES)"
	@echo "Headers: $(HEADERS)"
	@echo "Objects: $(OBJECTS)"
	@echo "Target: $(TARGET)"

.PHONY: clean rebuild run test debug
