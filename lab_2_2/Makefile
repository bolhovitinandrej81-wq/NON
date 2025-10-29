# Компилятор и флаги
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g
TARGET = crypto_system
SOURCES = main.cpp TableRouteCipher.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = TableRouteCipher.h

# Основная цель
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Компиляция объектных файлов
main.o: main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c main.cpp

TableRouteCipher.o: TableRouteCipher.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c TableRouteCipher.cpp

# Тестовые цели
test: $(TARGET)
	@echo "=== ТЕСТ 1: Пустой текст ==="
	@echo "" | ./$(TARGET) || true
	@echo ""
	
	@echo "=== ТЕСТ 2: Некорректные символы (цифры) ==="
	@echo -e "hello123\n3\n1" | ./$(TARGET) || true
	@echo ""
	
	@echo "=== ТЕСТ 3: Некорректные символы (русские) ==="
	@echo -e "ПРИВЕТ\n3\n1" | ./$(TARGET) || true
	@echo ""
	
	@echo "=== ТЕСТ 4: Ключ слишком маленький ==="
	@echo -e "hello\n1\n1" | ./$(TARGET) || true
	@echo ""
	
	@echo "=== ТЕСТ 5: Ключ слишком большой ==="
	@echo -e "hi\n5\n1" | ./$(TARGET) || true
	@echo ""
	
	@echo "=== ТЕСТ 6: Корректное шифрование ==="
	@echo -e "hello world\n4\n1\n2\n0" | ./$(TARGET) || true
	@echo ""
	
	@echo "=== ТЕСТ 7: Длинный текст ==="
	@echo -e "ALL ROBOTS HAVE MEMORY AND ARE\n11\n1\n2\n0" | ./$(TARGET) || true

# Быстрый тест (только компиляция)
quick: $(TARGET)
	@echo "Программа успешно скомпилирована!"

# Очистка
clean:
	rm -f $(TARGET) $(OBJECTS)

# Показать помощь
help:
	@echo "Доступные команды:"
	@echo "  make           - компиляция программы"
	@echo "  make test      - запуск всех тестов"
	@echo "  make quick     - быстрая проверка компиляции"
	@echo "  make clean     - очистка скомпилированных файлов"
	@echo "  make help      - показать эту справку"

.PHONY: all test quick clean help
