#include "modAlphaCipher.h"
#include <iostream>
#include <locale>
#include <string>
#include <codecvt>

using namespace std;

// Функции для преобразования string <-> wstring
wstring to_wide(const string& narrow_str) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(narrow_str);
}

string to_narrow(const wstring& wide_str) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wide_str);
}

bool isValid(const wstring& s)
{
    for(auto c : s) {
        if(c == L' ') continue;
        if((c < L'А' || c > L'Я') && c != L'Ё')
            return false;
    }
    return true;
}

bool isValidKey(const wstring& s)
{
    for(auto c : s) {
        if((c < L'А' || c > L'Я') && c != L'Ё')
            return false;
    }
    return true;
}

wstring to_upper_rus(const wstring& s) {
    wstring result = s;
    for (auto& c : result) {
        if (c >= L'а' && c <= L'я') {
            c = c - L'а' + L'А';
        } else if (c == L'ё') {
            c = L'Ё';
        }
    }
    return result;
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    string key_input;
    string text_input;
    unsigned op;
    
    cout << "Шифр готов. Введите ключ: ";
    getline(cin, key_input);
    
    // Преобразуем в wstring для проверок
    wstring key = to_wide(key_input);
    key = to_upper_rus(key);
    
    // Удаляем пробелы из ключа
    wstring key_no_spaces;
    for(auto c : key) {
        if(c != L' ') {
            key_no_spaces += c;
        }
    }
    key = key_no_spaces;
    
    try {
        // ТЕСТ 1: Пустой ключ
        if (key.empty()) {
            throw cipher_error("Пустой ключ");
        }
        
        // ТЕСТ 2: Неправильный ключ
        if (!isValidKey(key)) {
            throw cipher_error("Неправильный ключ: содержит недопустимые символы");
        }
        
        modAlphaCipher cipher(key);
        cout << "Ключ загружен" << endl;
        
        do {
            cout << "Выберите операцию (0-выход, 1-шифрование, 2-расшифрование): ";
            cin >> op;
            
            if(op > 2) {
                cout << "Неверная операция" << endl;
            } else if(op > 0) {
                cout << "Введите текст: ";
                cin.ignore();
                getline(cin, text_input);
                
                wstring text = to_wide(text_input);
                text = to_upper_rus(text);
                
                try {
                    if(op == 1) {
                        // ТЕСТ 3: Отсутствует текст (шифрование)
                        if (text.empty()) {
                            throw cipher_error("Отсутствует текст");
                        }
                        wstring encrypted = cipher.encrypt(text);
                        cout << "Зашифрованный текст: " << to_narrow(encrypted) << endl;
                    } else {
                        // ТЕСТ 4: Пустой текст (расшифрование)
                        if (text.empty()) {
                            throw cipher_error("Пустой текст");
                        }
                        
                        // ТЕСТ 5: Неправильно зашифрованный текст
                        if (!isValid(text)) {
                            throw cipher_error("Неправильно зашифрованный текст: содержит недопустимые символы");
                        }
                        
                        wstring decrypted = cipher.decrypt(text);
                        cout << "Расшифрованный текст: " << to_narrow(decrypted) << endl;
                    }
                } catch (const cipher_error& e) {
                    cout << "Ошибка: " << e.what() << endl;
                }
            }
        } while(op != 0);
        
    } catch (const cipher_error& e) {
        cerr << "Ошибка инициализации шифра: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
