#include "modAlphaCipher.h"
#include <iostream>
#include <locale>
#include <codecvt>
#include <algorithm>

using namespace std;

// Функция для проверки, содержит ли строка только русские буквы
bool contains_only_russian(const wstring& s) {
    for(auto c : s) {
        if((c < L'А' || c > L'Я') && c != L'Ё') {
            return false;
        }
    }
    return true;
}

wstring to_upper_rus(const wstring& s) {
    wstring result = s;
    for (auto& c : result) {
        if (c >= L'а' && c <= L'я') {
            c = c - L'а' + L'А';
        } 
        else if (c == L'ё') {
            c = L'Ё';
        }
    }
    return result;
}

wstring to_wide(const string& narrow_str) {
    try {
        wstring_convert<codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(narrow_str);
    } catch (const exception& e) {
        return L"";
    }
}

string to_narrow(const wstring& wide_str) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wide_str);
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    string key_input;
    string text_input;
    unsigned op;
    
    cout << "Шифр готов. Введите ключ: ";
    getline(cin, key_input);
    
    wstring key = to_wide(key_input);
    key = to_upper_rus(key);
    
    try {
        modAlphaCipher cipher(key);
        cout << "Ключ загружен\n";
        
        do {
            cout << "Шифр готов. Выберите операцию (0-выход, 1-шифрование, 2-расшифрование): ";
            cin >> op;
            
            if(op > 2) {
                cout << "Неверная операция\n";
            } else if(op > 0) {
                cout << "Введите текст: ";
                cin.ignore();
                getline(cin, text_input);
                
                wstring text = to_wide(text_input);
                if(text.empty() && !text_input.empty()) {
                    cout << "Ошибка кодировки\n";
                    continue;
                }
                
                text = to_upper_rus(text);
                
                try {
                    if(op == 1) {
                        wstring encrypted = cipher.encrypt(text);
                        cout << "Зашифрованный текст: " << to_narrow(encrypted) << endl;
                    } else {
                        wstring decrypted = cipher.decrypt(text);
                        cout << "Расшифрованный текст: " << to_narrow(decrypted) << endl;
                    }
                } catch (const cipher_error& e) {
                    cout << "Ошибка: " << e.what() << endl;
                }
            }
        } while(op != 0);
        
    } catch (const cipher_error& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    } catch (const exception& e) {
        cerr << "Неизвестная ошибка: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
