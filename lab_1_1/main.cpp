#include <iostream>
#include <string>
#include "TableRouteCipher.h"

using namespace std;

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
    
    string current_text = text;
    
    do {
        cout << "Шифратор готов. Выберите операцию (Выход-0, Шифровка-1, Расшифровка-2): ";
        cin >> vibor;
        
        if (vibor > 2) {
            cout << "Неверная операция!\n";
        } else if (vibor > 0) {
            if (vibor == 1) {
                current_text = shifr.encrypt(current_text);
                cout << "Зашифрованный текст: " << current_text << endl;
            } else {
                current_text = shifr.decrypt(current_text);
                cout << "Расшифрованный текст: " << current_text << endl;
            }
        }
    } while (vibor != 0);
    
    return 0;
}
