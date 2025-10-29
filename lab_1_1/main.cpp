#include <iostream>
#include <string>
#include <cctype>
#include "TableRouteCipher.h"

using namespace std;

bool isValidText(string &text) {
    for (char c : text) {
        if (!isalpha(c) && c != ' ') {
            return false;
        }
    }
    return true;
}

bool isValid(const int k, string &text) {
    if (k <= 0) {
        cout << "Ключ должен быть положительным числом\n";
        return false;
    }
    if (text.empty()) {
        cout << "Текст не может быть пустым\n";
        return false;
    }
    if (!isValidText(text)) {
        cout << "Текст содержит недопустимые символы\n";
        return false;
    }
    return true;
}

int main() {
    string text;
    int key;
    unsigned vibor;
    
    // Выводим информацию о поддерживаемых символах в самом начале
    cout << "--- Шифратор табличным маршрутным шифром ---\n";
    cout << "Поддерживаются только английские буквы и пробелы\n";
    cout << "---\n";
    
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
    
    do {
        cout << "Шифратор готов. Выберите операцию (Выход-0, Шифровка-1, Расшифровка-2): ";
        cin >> vibor;
        
        if (vibor > 2) {
            cout << "Неверная операция!\n";
        } else if (vibor > 0) {
            if (vibor == 1) {
                text = shifr.encrypt(text);
                cout << "Зашифрованный текст: " << text << endl;
            } else {
                text = shifr.decrypt(text);
                cout << "Расшифрованный текст: " << text << endl;
            }
        }
    } while (vibor != 0);
    
    return 0;
}
