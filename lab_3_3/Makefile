CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

# Цели
MAIN_TARGET = cipher
TEST_TARGET = test_TableRouteCipher

# Сборка основной программы
$(MAIN_TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

# Сборка тестов
$(TEST_TARGET): test_TableRouteCipher.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

# Запуск основной программы
run: $(MAIN_TARGET)
	./$(MAIN_TARGET)

# Запуск тестов
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Очистка
clean:
	rm -f $(MAIN_TARGET) $(TEST_TARGET)

.PHONY: run test clean
