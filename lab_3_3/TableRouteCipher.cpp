#include "TableRouteCipher.h"
#include <cctype>

using namespace std;

// Конструктор
TableRouteCipher::TableRouteCipher(int k) {
    if (k <= 0) {
        throw cipher_error("Ключ должен быть положительным числом");
    }
    key = k;
}

// Проверка ключа
inline int TableRouteCipher::getValidKey(int key, const string& Text) {
    if (key <= 0) {
        throw cipher_error("Ключ должен быть положительным");
    }
    if (key > (int)Text.length()) {
        throw cipher_error("Ключ слишком большой для текста");
    }
    return key;
}

// Проверка открытого текста
inline string TableRouteCipher::getValidOpenText(const string& s) {
    string result;
    
    if (s.empty()) {
        throw cipher_error("Текст не может быть пустым");
    }
    
    for (char c : s) {
        if (c != ' ') {
            if (!isalpha(c)) {
                throw cipher_error("Текст должен содержать только буквы");
            }
            result += c;
        }
    }
    
    if (result.empty()) {
        throw cipher_error("Текст должен содержать хотя бы одну букву");
    }
    
    return result;
}

// Проверка шифротекста
inline string TableRouteCipher::getValidCipherText(const string& s) {
    string result;
    
    if (s.empty()) {
        throw cipher_error("Шифротекст не может быть пустым");
    }
    
    for (char c : s) {
        if (c != ' ') {
            if (!isalpha(c)) {
                throw cipher_error("Шифротекст должен содержать только буквы");
            }
            result += c;
        }
    }
    
    if (result.empty()) {
        throw cipher_error("Шифротекст должен содержать хотя бы одну букву");
    }
    
    return result;
}

// Шифрование
string TableRouteCipher::encrypt(const string& text) {
    string clean_text = getValidOpenText(text);
    getValidKey(key, clean_text);
    
    int length = clean_text.length();
    int rows = (length + key - 1) / key;
    
    vector<vector<char>> table(rows, vector<char>(key, ' '));
    
    int index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < key; j++) {
            if (index < length) {
                table[i][j] = clean_text[index++];
            }
        }
    }
    
    string result;
    for (int j = 0; j < key; j++) {
        for (int i = 0; i < rows; i++) {
            if (table[i][j] != ' ') {
                result += table[i][j];
            }
        }
    }
    
    return result;
}

// Дешифрование
string TableRouteCipher::decrypt(const string& text) {
    string clean_text = getValidCipherText(text);
    
    if (key <= 0 || key > (int)clean_text.length()) {
        throw cipher_error("Некорректный ключ для данного текста");
    }
    
    int length = clean_text.length();
    int rows = (length + key - 1) / key;
    
    vector<vector<char>> table(rows, vector<char>(key, ' '));
    
    vector<int> col_counts(key, rows);
    int total_cells = rows * key;
    int empty_cells = total_cells - length;
    
    for (int j = key - 1; j >= key - empty_cells; j--) {
        col_counts[j] = rows - 1;
    }
    
    int text_index = 0;
    for (int j = 0; j < key; j++) {
        for (int i = 0; i < col_counts[j]; i++) {
            if (text_index < length) {
                table[i][j] = clean_text[text_index++];
            }
        }
    }
    
    string result;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < key; j++) {
            if (table[i][j] != ' ') {
                result += table[i][j];
            }
        }
    }
    
    return result;
}
