#pragma once
#include <map>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>

class modAlphaCipher
{
private:
    std::wstring numAlpha = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; // русский алфавит
    std::map<wchar_t, int> alphaNum; // ассоциативный массив "номер по символу"
    std::vector<int> key;         // ключ
    std::vector<int> convert(const std::wstring& s); // преобразование строка-вектор
    std::wstring convert(const std::vector<int>& v); // преобразование вектор-строка
    
public:
    modAlphaCipher() = delete;               // запретим конструктор без параметров
    modAlphaCipher(const std::wstring& skey); // конструктор для установки ключа
    std::wstring encrypt(const std::wstring& open_text);   // зашифрование
    std::wstring decrypt(const std::wstring& cipher_text); // расшифрование
};
