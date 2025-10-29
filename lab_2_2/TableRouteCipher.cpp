#include "TableRouteCipher.h"
#include <vector>
#include <iostream>
#include <cctype>

using namespace std;

TableRouteCipher::TableRouteCipher(int skey) { 
    key = skey; 
}

string TableRouteCipher::encrypt(string& text) {
    // Проверка 3.2.1: "Отсутствует открытый текст"
    string clean_text;
    for (char c : text) {
        if (c != ' ') {
            clean_text += c;
        }
    }
    
    if (clean_text.empty())
        throw cipher_error("Отсутствует открытый текст");

    // Проверка 3.2.2: "Некорректные символы в строке"
    for (char c : clean_text) {
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')))
            throw cipher_error("Некорректные символы в строке");
    }

    // Проверка 3.2.3: "Некорректный размер ключа" - ИСПРАВЛЕННЫЙ ПОРЯДОК
    // Сначала проверяем ключ, потом символы (как в оригинальных требованиях)
    if (key < 2)
        throw cipher_error("Некорректный размер ключа");
    
    // Исправлено: сравнение с преобразованием типа
    if (static_cast<size_t>(key) > clean_text.size())
        throw cipher_error("Некорректный размер ключа");

    int length = clean_text.length();
    int rows = (length + key - 1) / key;
    
    // Создаем таблицу и заполняем по строкам
    vector<vector<char>> table(rows, vector<char>(key, ' '));
    int index = 0;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < key; j++) {
            if (index < length) {
                table[i][j] = clean_text[index++];
            }
        }
    }
    
    // Читаем по столбцам СЛЕВА НАПРАВО
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

string TableRouteCipher::decrypt(string& text) {
    // Проверка 3.2.1: "Отсутствует открытый текст"
    string clean_text;
    for (char c : text) {
        if (c != ' ') {
            clean_text += c;
        }
    }
    
    if (clean_text.empty())
        throw cipher_error("Отсутствует открытый текст");

    // Проверка 3.2.2: "Некорректные символы в строке"
    for (char c : clean_text) {
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')))
            throw cipher_error("Некорректные символы в строке");
    }

    // Проверка 3.2.3: "Некорректный размер ключа" - ИСПРАВЛЕННЫЙ ПОРЯДОК
    if (key < 2)
        throw cipher_error("Некорректный размер ключа");
    
    // Исправлено: сравнение с преобразованием типа
    if (static_cast<size_t>(key) > clean_text.size())
        throw cipher_error("Некорректный размер ключа");

    int length = clean_text.length();
    int rows = (length + key - 1) / key;
    
    vector<vector<char>> table(rows, vector<char>(key, ' '));
    
    // Определяем количество символов в каждом столбце
    vector<int> col_counts(key, rows);
    int total_cells = rows * key;
    int empty_cells = total_cells - length;
    
    // Корректируем для последних столбцов (они могут быть короче)
    for (int j = key - 1; j >= key - empty_cells; j--) {
        col_counts[j] = rows - 1;
    }
    
    // Заполняем таблицу по столбцам
    int text_index = 0;
    for (int j = 0; j < key; j++) {
        for (int i = 0; i < col_counts[j]; i++) {
            if (text_index < length) {
                table[i][j] = clean_text[text_index++];
            }
        }
    }
    
    // Читаем по строкам для получения исходного текста
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
