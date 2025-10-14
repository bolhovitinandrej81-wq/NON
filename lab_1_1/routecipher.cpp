#include "routecipher.h"
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <algorithm>

using namespace std;

RouteCipher::RouteCipher(int key) : columns(key) 
{
    if (key <= 0) {
        throw invalid_argument("Error: Key must be a positive integer!");
    }
}

bool RouteCipher::isValidChar(char c) const
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

string RouteCipher::normalizeText(const string& text) const
{
    string result;
    for (char c : text) {
        if (isValidChar(c)) {
            result += toupper(c);
        }
    }
    return result;
}

int RouteCipher::calculateRows(int textLength) const
{
    return (textLength + columns - 1) / columns;
}

vector<vector<char>> RouteCipher::createEncryptionTable(const string& text) const
{
    int rows = calculateRows(text.length());
    vector<vector<char>> table(rows, vector<char>(columns, 'X'));
    int index = 0;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (index < text.length()) {
                table[i][j] = text[index++];
            }
        }
    }
    
    return table;
}

vector<vector<char>> RouteCipher::createDecryptionTable(const string& text) const
{
    int rows = calculateRows(text.length());
    int totalCells = rows * columns;
    
    string paddedText = text;
    if (paddedText.length() < totalCells) {
        paddedText += string(totalCells - paddedText.length(), 'X');
    }
    
    vector<vector<char>> table(rows, vector<char>(columns, ' '));
    int index = 0;
    
    for (int j = columns - 1; j >= 0; j--) {
        for (int i = 0; i < rows; i++) {
            if (index < paddedText.length()) {
                table[i][j] = paddedText[index++];
            }
        }
    }
    
    return table;
}

string RouteCipher::readHorizontal(const vector<vector<char>>& table) const
{
    string result;
    int rows = table.size();
    
    // Читаем построчно, но пропускаем 'X'
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (table[i][j] != 'X') {
                result += table[i][j];
            }
        }
    }
    
    return result;
}

string RouteCipher::readVerticalReverse(const vector<vector<char>>& table) const
{
    string result;
    int rows = table.size();
    
    // Читаем по столбцам справа налево ВСЕ символы (включая 'X')
    for (int j = columns - 1; j >= 0; j--) {
        for (int i = 0; i < rows; i++) {
            result += table[i][j];
        }
    }
    
    return result;
}

string RouteCipher::encrypt(const string& plainText)
{
    if (plainText.empty()) {
        throw invalid_argument("Error: Text cannot be empty!");
    }
    
    string normalized = normalizeText(plainText);
    if (normalized.empty()) {
        throw invalid_argument("Error: Text must contain only English letters!");
    }
    
    auto table = createEncryptionTable(normalized);
    return readVerticalReverse(table);
}

string RouteCipher::decrypt(const string& cipherText)
{
    if (cipherText.empty()) {
        throw invalid_argument("Error: Text cannot be empty!");
    }
    
    string normalized = normalizeText(cipherText);
    if (normalized.empty()) {
        throw invalid_argument("Error: Text must contain only English letters!");
    }
    
    auto table = createDecryptionTable(normalized);
    return readHorizontal(table);
}
