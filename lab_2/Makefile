# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
LDFLAGS = 

# Имена файлов
SOURCES = main.cpp modAlphaCipher.cpp
HEADERS = modAlphaCipher.h
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = crypto_system

# Правило по умолчанию
all: $(TARGET)

# Сборка основной программы
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Компиляция main.cpp
main.o: main.cpp modAlphaCipher.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

# Компиляция modAlphaCipher.cpp
modAlphaCipher.o: modAlphaCipher.cpp modAlphaCipher.h
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

# Проверка всех исключений (тестовый сценарий)
test: $(TARGET)
	@echo "=== ТЕСТ 1: Пустой ключ ==="
	@echo "" | ./$(TARGET) || true
	@echo ""
	
	@echo "=== ТЕСТ 2: Неправильный ключ (английские буквы) ==="
	@echo -e "HELLO\n0" | ./$(TARGET) || true
	@echo ""
	
	@echo "=== ТЕСТ 3: Неправильный ключ (цифры) ==="
	@echo -e "ПАРОЛЬ123\n0" | ./$(TARGET) || true
	@echo ""
	
	@echo "=== ТЕСТ 4: Отсутствие текста при шифровании ==="
	@echo -e "ПАРОЛЬ\n1\n\n0" | ./$(TARGET) || true
	@echo ""
	
	@echo "=== ТЕСТ 5: Пустой текст при расшифровании ==="
	@echo -e "ПАРОЛЬ\n2\n\n0" | ./$(TARGET) || true
	@echo ""
	
	@echo "=== ТЕСТ 6: Неправильно зашифрованный текст ==="
	@echo -e "ПАРОЛЬ\n2\nHELLO\n0" | ./$(TARGET) || true
	@echo ""
	
	@echo "=== ТЕСТ 7: Корректная работа ==="
	@echo -e "КЛЮЧ\n1\nПРИВЕТМИР\n2\nХЬФНЙЧЦОА\n0" | ./$(TARGET) || true

# Проверка стиля кода
check:
	@echo "Проверка стиля кода..."
	@echo "main.cpp:"
	@cat -n main.cpp | head -20
	@echo ""
	@echo "modAlphaCipher.cpp:"
	@cat -n modAlphaCipher.cpp | head -20
	@echo ""
	@echo "modAlphaCipher.h:"
	@cat -n modAlphaCipher.h

# Показать информацию о проекте
info:
	@echo "=== ИНФОРМАЦИЯ О ПРОЕКТЕ ==="
	@echo "Цель: $(TARGET)"
	@echo "Исходные файлы: $(SOURCES)"
	@echo "Заголовочные файлы: $(HEADERS)"
	@echo "Компилятор: $(CXX)"
	@echo "Флаги компиляции: $(CXXFLAGS)"
	@echo ""
	@echo "Доступные команды:"
	@echo "  make all      - собрать программу"
	@echo "  make clean    - очистить объектные файлы"
	@echo "  make rebuild  - пересобрать программу"
	@echo "  make run      - собрать и запустить программу"
	@echo "  make test     - запустить тесты исключений"
	@echo "  make debug    - собрать с отладочной информацией"
	@echo "  make check    - проверить стиль кода"
	@echo "  make info     - показать информацию о проекте"

# Файлы, которые не являются реальными файлами
.PHONY: all clean rebuild run test debug check info

# Помощь по умолчанию
help:
	@echo "Использование: make [цель]"
	@echo ""
	@echo "Цели:"
	@echo "  all      - собрать программу (по умолчанию)"
	@echo "  clean    - удалить объектные файлы и исполняемый файл"
	@echo "  rebuild  - пересобрать программу"
	@echo "  run      - собрать и запустить программу"
	@echo "  test     - запустить тесты всех исключений"
	@echo "  debug    - собрать с отладочной информацией"
	@echo "  check    - проверить стиль кода"
	@echo "  info     - показать информацию о проекте"
	@echo "  help     - показать эту справку"
