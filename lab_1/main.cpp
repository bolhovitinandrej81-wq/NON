#include "modAlphaCipher.h"
#include <iostream>
#include <locale>
#include <codecvt>
#include <algorithm>

using namespace std;

bool isValid(const wstring& s)
{
    for(auto c : s) {
        // Разрешаем пробелы в дополнение к русским буквам
        if(c == L' ') {
            continue;
        }
        if(c < L'А' || c > L'Я') {
            if(c != L'Ё')
                return false;
        }
    }
    return true;
}

bool isValidKey(const wstring& s)
{
    for(auto c : s) {
        // Для ключа пробелы не разрешаем
        if(c < L'А' || c > L'Я') {
            if(c != L'Ё')
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
        // Ё -> Ё
        else if (c == L'ё') {
            c = L'Ё';
        }
        // пробелы остаются как есть
    }
    return result;
}

wstring to_wide(const string& narrow_str) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(narrow_str);
}

string to_narrow(const wstring& wide_str) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wide_str);
}

// Функция для удаления пробелов из текста
wstring remove_spaces(const wstring& s) {
    wstring result;
    for(auto c : s) {
        if(c != L' ') {
            result += c;
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
    
    wstring key = to_wide(key_input);
    key = to_upper_rus(key);
    
    // Удаляем пробелы из ключа (в ключе пробелы недопустимы)
    wstring key_no_spaces;
    for(auto c : key) {
        if(c != L' ') {
            key_no_spaces += c;
        }
    }
    key = key_no_spaces;
    
    if(!isValidKey(key) || key.empty()) {
        cerr << "Ключ недействителен! Используйте только русские буквы.\n";
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
            cin.ignore();
            getline(cin, text_input);
            
            wstring text = to_wide(text_input);
            text = to_upper_rus(text);
            
            if(isValid(text)) {
                if(op == 1) {
                    // Удаляем пробелы перед шифрованием
                    wstring text_no_spaces = remove_spaces(text);
                    wstring encrypted = cipher.encrypt(text_no_spaces);
                    cout << "Зашифрованный текст: " << to_narrow(encrypted) << endl;
                } else {
                    // Удаляем пробелы перед расшифрованием
                    wstring text_no_spaces = remove_spaces(text);
                    wstring decrypted = cipher.decrypt(text_no_spaces);
                    cout << "Расшифрованный текст: " << to_narrow(decrypted) << endl;
                }
            } else {
                cout << "Операция отменена: неверный текст. Используйте только русские буквы и пробелы.\n";
            }
        }
    } while(op != 0);
    
    return 0;
}
