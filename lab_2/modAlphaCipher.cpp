#include "modAlphaCipher.h"
#include <locale>
#include <codecvt>
#include <cwctype>
#include <algorithm>

using namespace std;

modAlphaCipher::modAlphaCipher(const wstring& skey)
{
    // Инициализация алфавита и сопоставления символов с номерами
    for(unsigned i = 0; i < numAlpha.size(); i++) {
        alphaNum[numAlpha[i]] = i;
    }
    
    // Проверка ключа перед использованием
    if (skey.empty()) {
        throw key_error("Ключ не может быть пустым"); // Добавлено исключение для пустого ключа
    }
    
    key = convert(skey);
    
    // Дополнительная проверка: ключ не должен состоять только из пробелов
    bool has_valid_chars = false;
    for (auto k : key) {
        if (k != -1) { // -1 означает пробел
            has_valid_chars = true;
            break;
        }
    }
    if (!has_valid_chars) {
        throw key_error("Ключ должен содержать хотя бы одну русскую букву"); // Добавлено исключение
    }
}

wstring modAlphaCipher::encrypt(const wstring& open_text)
{
    // Проверка входного текста
    if (open_text.empty()) {
        throw text_error("Текст для шифрования не может быть пустым"); // Добавлено исключение
    }
    
    vector<int> work = convert(open_text);
    int key_index = 0; // отдельный индекс для ключа
    
    for(unsigned i = 0; i < work.size(); i++) {
        if(work[i] != -1) { // если не пробел
            work[i] = (work[i] + key[key_index % key.size()]) % alphaNum.size();
            key_index++; // увеличиваем индекс ключа только когда шифруем букву
        }
    }
    return convert(work);
}

wstring modAlphaCipher::decrypt(const wstring& cipher_text)
{
    // Проверка входного текста
    if (cipher_text.empty()) {
        throw text_error("Текст для расшифрования не может быть пустым"); // Добавлено исключение
    }
    
    vector<int> work = convert(cipher_text);
    int key_index = 0; // отдельный индекс для ключа
    
    for(unsigned i = 0; i < work.size(); i++) {
        if(work[i] != -1) { // если не пробел
            work[i] = (work[i] + alphaNum.size() - key[key_index % key.size()]) % alphaNum.size();
            key_index++; // увеличиваем индекс ключа только когда расшифровываем букву
        }
    }
    return convert(work);
}

vector<int> modAlphaCipher::convert(const wstring& s)
{
    vector<int> result;
    for(auto c : s) {
        if(c == L' ') {
            // Пробелы помечаем как -1
            result.push_back(-1);
        } else {
            // Проверяем, что символ есть в алфавите
            if (alphaNum.find(c) == alphaNum.end()) {
                throw text_error("Обнаружен недопустимый символ в тексте"); // Добавлено исключение
            }
            result.push_back(alphaNum[c]);
        }
    }
    return result;
}

wstring modAlphaCipher::convert(const vector<int>& v)
{
    wstring result;
    for(auto i : v) {
        if(i == -1) {
            // Восстанавливаем пробелы
            result.push_back(L' ');
        } else {
            // Проверяем, что индекс в допустимом диапазоне
            if (i < 0 || i >= static_cast<int>(numAlpha.size())) {
                throw cipher_error("Недопустимый индекс символа при преобразовании"); // Добавлено исключение
            }
            result.push_back(numAlpha[i]);
        }
    }
    return result;
}
