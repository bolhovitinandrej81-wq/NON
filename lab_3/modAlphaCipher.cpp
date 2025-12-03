#include "modAlphaCipher.h"
#include <algorithm>
#include <iostream>

using namespace std;

// Конструктор для wstring
modAlphaCipher::modAlphaCipher(const wstring& skey) {
    if (!isValidKey(skey)) {
        throw cipher_error("Недопустимый ключ");
    }
    
    // Инициализация alphaNum
    for (size_t i = 0; i < numAlpha.size(); i++) {
        alphaNum[numAlpha[i]] = i;
    }
    
    // Преобразование ключа
    wstring upper_key = to_upper_rus(skey);
    key = convert(upper_key);
    
    // Проверка на слабый ключ (все одинаковые символы)
    if (key.size() > 0) {
        bool all_same = true;
        int first = key[0];
        for (size_t i = 1; i < key.size(); i++) {
            if (key[i] != first) {
                all_same = false;
                break;
            }
        }
        if (all_same) {
            throw cipher_error("Слабый ключ (все символы одинаковые)");
        }
    }
}

// Шифрование для wstring
wstring modAlphaCipher::encrypt(const wstring& open_text) {
    if (open_text.empty()) {
        throw cipher_error("Пустой открытый текст");
    }
    
    // Преобразование текста
    wstring processed_text = to_upper_rus(open_text);
    
    // Удаление не-буквенных символов (кроме пробелов)
    wstring cleaned_text;
    for (wchar_t c : processed_text) {
        if ((c >= L'А' && c <= L'Я') || c == L'Ё' || 
            (c >= L'а' && c <= L'я') || c == L'ё' || c == L' ') {
            cleaned_text += c;
        }
        // Цифры и другие символы игнорируются
    }
    
    // Проверка, остались ли русские буквы
    bool has_russian = false;
    for (wchar_t c : cleaned_text) {
        if ((c >= L'А' && c <= L'Я') || c == L'Ё' || 
            (c >= L'а' && c <= L'я') || c == L'ё') {
            has_russian = true;
            break;
        }
    }
    
    if (!has_russian) {
        throw cipher_error("Текст не содержит русских букв");
    }
    
    // Удаление пробелов для шифрования
    wstring text_no_spaces;
    for (wchar_t c : cleaned_text) {
        if (c != L' ') {
            text_no_spaces += c;
        }
    }
    
    // Преобразование к верхнему регистру
    text_no_spaces = to_upper_rus(text_no_spaces);
    
    // Проверка на допустимые символы
    if (!isValidText(text_no_spaces, false)) {
        throw cipher_error("Текст содержит недопустимые символы");
    }
    
    // Шифрование
    vector<int> work = convert(text_no_spaces);
    for (size_t i = 0; i < work.size(); i++) {
        work[i] = (work[i] + key[i % key.size()]) % numAlpha.size();
    }
    
    return convert(work);
}

// Дешифрование для wstring
wstring modAlphaCipher::decrypt(const wstring& cipher_text) {
    if (cipher_text.empty()) {
        throw cipher_error("Пустой шифротекст");
    }
    
    // Проверка на допустимые символы (только заглавные русские буквы)
    if (!isValidText(cipher_text, false)) {
        throw cipher_error("Шифротекст содержит недопустимые символы");
    }
    
    // Дешифрование
    vector<int> work = convert(cipher_text);
    for (size_t i = 0; i < work.size(); i++) {
        work[i] = (work[i] + numAlpha.size() - key[i % key.size()]) % numAlpha.size();
    }
    
    return convert(work);
}

// Перегрузки для string
string modAlphaCipher::encrypt(const string& open_text) {
    wstring wtext = s2ws(open_text);
    wstring wresult = encrypt(wtext);
    return ws2s(wresult);
}

string modAlphaCipher::decrypt(const string& cipher_text) {
    wstring wtext = s2ws(cipher_text);
    wstring wresult = decrypt(wtext);
    return ws2s(wresult);
}

// Вспомогательные методы
vector<int> modAlphaCipher::convert(const wstring& s) {
    vector<int> result;
    for (wchar_t c : s) {
        result.push_back(alphaNum[c]);
    }
    return result;
}

wstring modAlphaCipher::convert(const vector<int>& v) {
    wstring result;
    for (int i : v) {
        result.push_back(numAlpha[i]);
    }
    return result;
}

wstring modAlphaCipher::to_upper_rus(const wstring& s) {
    wstring result = s;
    for (wchar_t& c : result) {
        if (c >= L'а' && c <= L'я') {
            c = L'А' + (c - L'а');
        } else if (c == L'ё') {
            c = L'Ё';
        }
    }
    return result;
}

bool modAlphaCipher::isValidKey(const wstring& s) {
    if (s.empty()) {
        return false;
    }
    
    wstring upper_key = to_upper_rus(s);
    
    for (wchar_t c : upper_key) {
        // Проверяем только русские буквы
        if ((c < L'А' || c > L'Я') && c != L'Ё') {
            return false;
        }
    }
    
    return true;
}

bool modAlphaCipher::isValidText(const wstring& s, bool allowLowercase) {
    if (s.empty()) {
        return false;
    }
    
    for (wchar_t c : s) {
        if (allowLowercase) {
            // Разрешаем строчные и заглавные русские буквы
            if (!((c >= L'А' && c <= L'Я') || c == L'Ё' || 
                  (c >= L'а' && c <= L'я') || c == L'ё')) {
                return false;
            }
        } else {
            // Только заглавные русские буквы
            if (!((c >= L'А' && c <= L'Я') || c == L'Ё')) {
                return false;
            }
        }
    }
    
    return true;
}
