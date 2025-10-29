#include "modAlphaCipher.h"
#include <iostream>
#include <locale>
#include <codecvt>
#include <algorithm>

using namespace std;

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

// Функция для проверки, содержит ли строка только русские буквы и пробелы
bool is_valid_russian_text(const wstring& s) {
    for(auto c : s) {
        if(c == L' ') continue; // Пропускаем пробелы
        if((c < L'А' || c > L'Я') && c != L'Ё') {
            return false;
        }
    }
    return true;
}

// Функция для проверки ключа (только русские буквы, без пробелов)
bool is_valid_key(const wstring& s) {
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
    
    // Удаляем пробелы из ключа
    wstring clean_key = remove_spaces(key);
    
    try {
        // Проверяем ключ перед созданием шифра
        if(!is_valid_key(clean_key)) {
            throw cipher_error("Неправильный ключ: содержит недопустимые символы");
        }
        
        modAlphaCipher cipher(clean_key);
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
                        // Для шифрования: проверяем текст и удаляем пробелы
                        if(!is_valid_russian_text(text)) {
                            cout << "Ошибка: Текст содержит недопустимые символы\n";
                            continue;
                        }
                        wstring text_no_spaces = remove_spaces(text);
                        wstring encrypted = cipher.encrypt(text_no_spaces);
                        cout << "Зашифрованный текст: " << to_narrow(encrypted) << endl;
                    } else {
                        // Для расшифрования: удаляем пробелы и передаем в decrypt
                        wstring text_no_spaces = remove_spaces(text);
                        wstring decrypted = cipher.decrypt(text_no_spaces);
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
