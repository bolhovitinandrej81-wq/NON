#pragma once
#include <map>
#include <string>
#include <vector>

class modAlphaCipher
{
private:
    std::string numAlpha = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; // русский алфавит
    std::map<char, int> alphaNum; // ассоциативный массив "номер по символу"
    std::vector<int> key;         // ключ
    std::vector<int> convert(const std::string& s); // преобразование строка-вектор
    std::string convert(const std::vector<int>& v); // преобразование вектор-строка
    
public:
    modAlphaCipher() = delete;               // запретим конструктор без параметров
    modAlphaCipher(const std::string& skey); // конструктор для установки ключа
    std::string encrypt(const std::string& open_text);   // зашифрование
    std::string decrypt(const std::string& cipher_text); // расшифрование
};
