#include <iostream>
#include <string>
#include <cctype>
#include "TableRouteCipher.h"

using namespace std;

// Упрощенная валидация - проверяем только базовые условия
bool isValid(const int k, string &text) {
    if (k <= 0) {
        cout << "Ключ должен быть положительным числом\n";
        return false;
    }
    if (text.empty()) {
        cout << "Текст не может быть пустым\n";
        return false;
    }
    return true;
}

int main() {
    string text;
    int key;
    unsigned vibor;
    
    // Выводим информацию о поддерживаемых символах в самом начале
    cout << "=== Шифратор табличным маршрутным шифром ===\n";
    cout << "Поддерживаются только английские буквы и пробелы\n";
    cout << "=============================================\n\n";
    
    cout << "Введите текст: ";
    getline(cin, text);
    
    cout << "Введите кол-во столбцов: ";
    cin >> key;
    
    if (!isValid(key, text)) {
        cout << "Ключ не корректен\n";
        return 1;
    }
    
    cout << "Ключ загружен\n";
    TableRouteCipher shifr(key);
    
    string original_text = text; // Сохраняем оригинальный текст с пробелами
    string current_text = text;
    
    do {
        cout << "Шифратор готов. Выберите операцию (Выход-0, Шифровка-1, Расшифровка-2): ";
        cin >> vibor;
        
        if (vibor > 2) {
            cout << "Неверная операция!\n";
        } else if (vibor > 0) {
            try {
                if (vibor == 1) {
                    current_text = shifr.encrypt(current_text);
                    cout << "Зашифрованный текст: " << current_text << endl;
                } else {
                    current_text = shifr.decrypt(current_text);
                    
                    // Восстанавливаем пробелы из оригинального текста
                    string result_with_spaces;
                    int decr_index = 0;
                    
                    for (size_t i = 0; i < original_text.length(); i++) {
                        if (original_text[i] == ' ') {
                            result_with_spaces += ' ';
                        } else {
                            if (static_cast<size_t>(decr_index) < current_text.length()) {
                                result_with_spaces += current_text[decr_index++];
                            }
                        }
                    }
                    
                    cout << "Расшифрованный текст: " << result_with_spaces << endl;
                    current_text = result_with_spaces; // Сохраняем текст с пробелами для следующих операций
                }
            } catch (const cipher_error& e) {
                cout << "Ошибка: " << e.what() << endl;
            } catch (const exception& e) {
                cout << "Неизвестная ошибка: " << e.what() << endl;
            }
        }
    } while (vibor != 0);
    
    return 0;
}
