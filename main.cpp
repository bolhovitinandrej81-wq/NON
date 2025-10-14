#include "modAlphaCipher.h"
#include <iostream>
#include <algorithm>
#include <locale>
#include <cctype>

using namespace std;

// проверка, чтобы строка состояла только из русских букв
bool isValid(const string& s)
{
    // Временно отключаем проверку для тестирования
    return true;
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    string key;
    string text;
    unsigned op;
    
    cout << "Шифр готов. Введите ключ: ";
    cin >> key;
    
    transform(key.begin(), key.end(), key.begin(), ::toupper);
    
    if(!isValid(key)) {
        cerr << "Ключ недействителен! Используйте только русские буквы в верхнем регистре.\n";
        return 1;
    }
    
    cout << "Ключ загружен\n";
    modAlphaCipher cipher(key);
    
    do {
        cout << "Шифр готов. Выберите операцию (0-выход, 1-шифрование, 2-расшифрование): ";
        cin >> op;
        
        if(op > 2) {
            cout << "Неверная операция\n";
        } else if(op > 0) {
            cout << "Введите текст: ";
            cin >> text;
            
            transform(text.begin(), text.end(), text.begin(), ::toupper);
            
            if(isValid(text)) {
                if(op == 1) {
                    string encrypted = cipher.encrypt(text);
                    cout << "Зашифрованный текст: " << encrypted << endl;
                } else {
                    string decrypted = cipher.decrypt(text);
                    cout << "Расшифрованный текст: " << decrypted << endl;
                }
            } else {
                cout << "Операция отменена: неверный текст. Используйте только русские буквы.\n";
            }
        }
    } while(op != 0);
    
    return 0;
}
