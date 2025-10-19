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
    
    // Оборачиваем создание шифра в блок try-catch для обработки исключений
    modAlphaCipher* cipher = nullptr;
    try {
        cipher = new modAlphaCipher(key);
        cout << "Шифр успешно инициализирован\n";
    } catch (const key_error& e) {
        cerr << "Ошибка ключа: " << e.what() << endl;
        return 1;
    } catch (const cipher_error& e) {
        cerr << "Ошибка шифра: " << e.what() << endl;
        return 1;
    } catch (const exception& e) {
        cerr << "Неизвестная ошибка: " << e.what() << endl;
        return 1;
    }
    
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
                try {
                    if(op == 1) {
                        wstring encrypted = cipher->encrypt(text);
                        cout << "Зашифрованный текст: " << to_narrow(encrypted) << endl;
                    } else {
                        wstring decrypted = cipher->decrypt(text);
                        cout << "Расшифрованный текст: " << to_narrow(decrypted) << endl;
                    }
                } catch (const text_error& e) {
                    cout << "Ошибка текста: " << e.what() << endl;
                } catch (const cipher_error& e) {
                    cout << "Ошибка шифрования: " << e.what() << endl;
                } catch (const exception& e) {
                    cout << "Неизвестная ошибка: " << e.what() << endl;
                }
            } else {
                cout << "Операция отменена: неверный текст. Используйте только русские буквы и пробелы.\n";
            }
        }
    } while(op != 0);
    
    delete cipher; // Освобождаем память
    return 0;
}
