#pragma once
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <codecvt>
#include <locale>

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
    std::wstring to_upper_rus(const std::wstring& s);
    bool isValidKey(const std::wstring& s);
    bool isValidText(const std::wstring& s, bool allowLowercase = false);
    
    // Конвертеры
    static std::wstring s2ws(const std::string& str) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(str);
    }
    
    static std::string ws2s(const std::wstring& wstr) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(wstr);
    }
    
public:
    modAlphaCipher() = delete;
    modAlphaCipher(const std::wstring& skey);
    modAlphaCipher(const std::string& skey);
    
    // Основные методы
    std::wstring encrypt(const std::wstring& open_text);
    std::string encrypt(const std::string& open_text);
    std::wstring decrypt(const std::wstring& cipher_text);
    std::string decrypt(const std::string& cipher_text);
};
