CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Имя исполняемого файла
TARGET = test_tableroutecipher

# Исходный файл
SOURCE = TableRouteCipher.cpp

# Правило для сборки исполняемого файла
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

# Правило для запуска тестов (предполагает, что исполняемый файл уже собран)
test: $(TARGET)
	./$(TARGET)

# Правило для очистки (удаления исполняемого файла)
clean:
	rm -f $(TARGET)

# Правило по умолчанию - сборка
all: $(TARGET)

.PHONY: test clean all
