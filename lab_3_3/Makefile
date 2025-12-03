# Makefile для TableRouteCipher проекта

# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic
LDFLAGS = -lUnitTest++

# Имена файлов
MAIN_SOURCE = TableRouteCipher.cpp
TEST_SOURCE = main.cpp
HEADER = TableRouteCipher.h

# Целевой файл
TARGET = TableRouteCipherTest

# Основные цели
.PHONY: all build test clean run help

# Основная цель
all: build test

# Сборка
build: $(TARGET)

$(TARGET): $(MAIN_SOURCE) $(TEST_SOURCE) $(HEADER)
	@echo "🔧 Компиляция проекта..."
	$(CXX) $(CXXFLAGS) $(MAIN_SOURCE) $(TEST_SOURCE) -o $(TARGET) $(LDFLAGS)
	@echo "✅ Сборка завершена: $(TARGET)"

# Запуск тестов
test: build
	@echo ""
	@echo "🧪 ЗАПУСК ТЕСТОВ"
	@echo "════════════════════════════════════════"
	@./$(TARGET)
	@echo "════════════════════════════════════════"

# Псевдоним
run: test

# Очистка
clean:
	@echo "🧹 Очистка..."
	@rm -f $(TARGET) *.o
	@echo "✅ Очистка завершена"

# Простая проверка
check:
	@echo "📝 Проверка файлов..."
	@ls -la $(MAIN_SOURCE) $(TEST_SOURCE) $(HEADER) 2>/dev/null || echo "❌ Некоторые файлы отсутствуют"
	@echo ""
	@echo "📝 Проверка компиляции..."
	@$(CXX) $(CXXFLAGS) -c $(MAIN_SOURCE) -o /tmp/test.o 2>&1 && echo "✅ TableRouteCipher.cpp компилируется" || echo "❌ Ошибка в TableRouteCipher.cpp"

# Справка
help:
	@echo "📖 ДОСТУПНЫЕ КОМАНДЫ:"
	@echo "  make all     - собрать и запустить тесты"
	@echo "  make build   - только сборка"
	@echo "  make test    - сборка и запуск тестов"
	@echo "  make run     - то же что и test"
	@echo "  make clean   - очистка"
	@echo "  make check   - проверить файлы и компиляцию"
	@echo "  make help    - эта справка"

# Правило по умолчанию
.DEFAULT_GOAL := help
