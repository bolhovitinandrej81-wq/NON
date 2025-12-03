#pragma once
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <locale>
#include <codecvt>

class cipher_error: public std::invalid_argument {
public:
    explicit cipher_error(const std::string& what_arg): 
        std::invalid_argument(what_arg) {}
    explicit cipher_error(const char* what_arg): 
        std::invalid_argument(what_arg) {}
};

class modAlphaCipher {
private:
    std::wstring numAlpha = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    std::map<wchar_t,int> alphaNum;
    std::vector<int> key;
    
    // Вспомогательные методы
    std::vector<int> convert(const std::wstring& s);
    std::wstring convert(const std::vector<int>& v);
    
public:
    modAlphaCipher() = delete;
    explicit modAlphaCipher(const std::string& skey);
    
    // Основные методы
    std::string encrypt(const std::string& open_text);
    std::string decrypt(const std::string& cipher_text);
    
private:
    // Вспомогательные функции
    std::wstring to_upper_rus(const std::wstring& s);
    bool isValidKey(const std::string& s);
    bool isValidText(const std::string& s, bool allowLowercase = false);
    std::wstring s2ws(const std::string& str);
    std::string ws2s(const std::wstring& wstr);
    std::string processTextForEncrypt(const std::string& text);
    std::string processTextForDecrypt(const std::string& text);
};
