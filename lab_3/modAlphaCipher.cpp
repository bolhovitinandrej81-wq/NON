#include "modAlphaCipher.h"
#include <algorithm>
#include <iostream>
#include <clocale>
#include <cctype>

using namespace std;

// Конвертеры с использованием codecvt
wstring modAlphaCipher::s2ws(const string& str) {
    try {
        wstring_convert<codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(str);
    } catch (...) {
        return L"";
    }
}

string modAlphaCipher::ws2s(const wstring& wstr) {
    try {
        wstring_convert<codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(wstr);
    } catch (...) {
        return "";
    }
}

// Конструктор
modAlphaCipher::modAlphaCipher(const string& skey) {
    if (!isValidKey(skey)) {
        throw cipher_error("Недопустимый ключ");
    }
    
    // Инициализация alphaNum
    for (size_t i = 0; i < numAlpha.size(); i++) {
        alphaNum[numAlpha[i]] = i;
    }
    
    // Преобразование ключа
    wstring wkey = s2ws(skey);
    wstring upper_key = to_upper_rus(wkey);
    key = convert(upper_key);
    
    // Проверка на слабый ключ (все одинаковые символы)
    // Ключ из одного символа НЕ считается слабым
    if (key.size() > 1) {
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

// Шифрование
string modAlphaCipher::encrypt(const string& open_text) {
    if (open_text.empty()) {
        throw cipher_error("Пустой открытый текст");
    }
    
    string processed_text = processTextForEncrypt(open_text);
    
    if (processed_text.empty()) {
        throw cipher_error("Текст не содержит русских букв");
    }
    
    // Преобразование текста
    wstring wtext = s2ws(processed_text);
    wstring upper_text = to_upper_rus(wtext);
    
    // Проверка на допустимые символы
    for (wchar_t c : upper_text) {
        if ((c < L'А' || c > L'Я') && c != L'Ё') {
            throw cipher_error("Текст содержит недопустимые символы");
        }
    }
    
    // Шифрование
    vector<int> work = convert(upper_text);
    for (size_t i = 0; i < work.size(); i++) {
        work[i] = (work[i] + key[i % key.size()]) % numAlpha.size();
    }
    
    wstring wresult = convert(work);
    return ws2s(wresult);
}

// Дешифрование
string modAlphaCipher::decrypt(const string& cipher_text) {
    if (cipher_text.empty()) {
        throw cipher_error("Пустой шифротекст");
    }
    
    string processed_text = processTextForDecrypt(cipher_text);
    
    // Преобразование текста
    wstring wtext = s2ws(processed_text);
    
    // Проверка на допустимые символы (только заглавные русские буквы)
    for (wchar_t c : wtext) {
        if ((c < L'А' || c > L'Я') && c != L'Ё') {
            throw cipher_error("Шифротекст содержит недопустимые символы");
        }
    }
    
    // Дешифрование
    vector<int> work = convert(wtext);
    for (size_t i = 0; i < work.size(); i++) {
        work[i] = (work[i] + numAlpha.size() - key[i % key.size()]) % numAlpha.size();
    }
    
    wstring wresult = convert(work);
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

bool modAlphaCipher::isValidKey(const string& s) {
    if (s.empty()) {
        return false;
    }
    
    wstring wkey = s2ws(s);
    wstring upper_key = to_upper_rus(wkey);
    
    for (wchar_t c : upper_key) {
        // Проверяем только русские буквы
        if ((c < L'А' || c > L'Я') && c != L'Ё') {
            return false;
        }
    }
    
    return true;
}

bool modAlphaCipher::isValidText(const string& s, bool allowLowercase) {
    if (s.empty()) {
        return false;
    }
    
    wstring wtext = s2ws(s);
    
    for (wchar_t c : wtext) {
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

string modAlphaCipher::processTextForEncrypt(const string& text) {
    // Преобразуем в wstring для обработки
    wstring wtext = s2ws(text);
    
    // Проверяем наличие только русских букв и пробелов
    for (wchar_t c : wtext) {
        if (c != L' ') {
            // Проверяем русские буквы (строчные и заглавные)
            if (!((c >= L'А' && c <= L'Я') || c == L'Ё' || 
                  (c >= L'а' && c <= L'я') || c == L'ё')) {
                // Если символ не пробел и не русская буква - исключение
                throw cipher_error("Текст содержит недопустимые символы");
            }
        }
    }
    
    // Удаление пробелов и преобразование к верхнему регистру
    wstring cleaned_text;
    for (wchar_t c : wtext) {
        if (c != L' ') {
            if (c >= L'а' && c <= L'я') {
                c = L'А' + (c - L'а');
            } else if (c == L'ё') {
                c = L'Ё';
            }
            cleaned_text += c;
        }
    }
    
    return ws2s(cleaned_text);
}

string modAlphaCipher::processTextForDecrypt(const string& text) {
    // Для дешифрования просто возвращаем текст как есть
    // (проверка символов будет в decrypt)
    return text;
}
