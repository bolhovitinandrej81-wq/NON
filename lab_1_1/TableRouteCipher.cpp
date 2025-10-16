#include "TableRouteCipher.h"
#include <vector>
#include <iostream>

using namespace std;

TableRouteCipher::TableRouteCipher(int skey) { 
    key = skey; 
}

string TableRouteCipher::encrypt(string& text) {
    int length = text.length();
    int rows = (length + key - 1) / key;
    
    vector<vector<char>> table(rows, vector<char>(key, ' '));
    
    // Заполняем по строкам
    int index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < key; j++) {
            if (index < length) {
                table[i][j] = text[index++];
            }
        }
    }
    
    // Читаем змейкой: четные столбцы сверху вниз, нечетные снизу вверх
    string result;
    for (int j = 0; j < key; j++) {
        if (j % 2 == 0) { // Четные столбцы
            for (int i = 0; i < rows; i++) {
                if (table[i][j] != ' ') {
                    result += table[i][j];
                }
            }
        } else { // Нечетные столбцы
            for (int i = rows - 1; i >= 0; i--) {
                if (table[i][j] != ' ') {
                    result += table[i][j];
                }
            }
        }
    }
    
    return result;
}

string TableRouteCipher::decrypt(string& text) {
    int length = text.length();
    int rows = (length + key - 1) / key;
    
    vector<vector<char>> table(rows, vector<char>(key, ' '));
    
    // Заполняем змейкой
    int index = 0;
    for (int j = 0; j < key; j++) {
        if (j % 2 == 0) {
            for (int i = 0; i < rows; i++) {
                if (index < length) {
                    table[i][j] = text[index++];
                }
            }
        } else {
            for (int i = rows - 1; i >= 0; i--) {
                if (index < length) {
                    table[i][j] = text[index++];
                }
            }
        }
    }
    
    // Читаем по строкам
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
