#include "TableRouteCipher.h"
#include <vector>
#include <iostream>

using namespace std;

TableRouteCipher::TableRouteCipher(int skey) { 
    key = skey; 
}

string TableRouteCipher::encrypt(string& text) {
    // Удаляем пробелы для шифрования
    string clean_text;
    for (char c : text) {
        if (c != ' ') {
            clean_text += c;
        }
    }
    
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
    int length = text.length();
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
                table[i][j] = text[text_index++];
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
