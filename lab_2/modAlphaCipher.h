#pragma once
#include <map>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>
#include <stdexcept> // Добавляем для обработки исключений

// Класс для шифрования методом Гронсвельда (модифицированный шифр Виженера)
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

// Классы исключений для более точной обработки ошибок
class cipher_error : public std::invalid_argument {
public:
    explicit cipher_error(const std::string& what_arg) : std::invalid_argument(what_arg) {}
    explicit cipher_error(const char* what_arg) : std::invalid_argument(what_arg) {}
};

class key_error : public cipher_error {
public:
    explicit key_error(const std::string& what_arg) : cipher_error(what_arg) {}
    explicit key_error(const char* what_arg) : cipher_error(what_arg) {}
};

class text_error : public cipher_error {
public:
    explicit text_error(const std::string& what_arg) : cipher_error(what_arg) {}
    explicit text_error(const char* what_arg) : cipher_error(what_arg) {}
};
