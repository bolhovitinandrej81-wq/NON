#include "modAlphaCipher.h"
#include <locale>
#include <codecvt>
#include <cwctype>
#include <algorithm>

using namespace std;

modAlphaCipher::modAlphaCipher(const wstring& skey)
{
    // 3.1.1 Пустой ключ
    if(skey.empty())
        throw cipher_error("Пустой ключ");

    // 3.1.2 Неправильный ключ - проверяем на русские буквы (без пробелов)
    for(auto c : skey) {
        // Разрешаем только русские буквы (А-Я и Ё)
        if((c < L'А' || c > L'Я') && c != L'Ё') {
            throw cipher_error("Неправильный ключ: содержит недопустимые символы");
        }
    }

    for(unsigned i = 0; i < numAlpha.size(); i++) {
        alphaNum[numAlpha[i]] = i;
    }
    key = convert(skey);
}

wstring modAlphaCipher::encrypt(const wstring& open_text)
{
    // 3.1.3 Отсутствие текста
    if(open_text.empty())
        throw cipher_error("Отсутствует текст");

    vector<int> work = convert(open_text);
    
    for(unsigned i = 0; i < work.size(); i++) {
        work[i] = (work[i] + key[i % key.size()]) % alphaNum.size();
    }
    return convert(work);
}

wstring modAlphaCipher::decrypt(const wstring& cipher_text)
{
    // 3.1.4 Пустой текст
    if(cipher_text.empty())
        throw cipher_error("Пустой текст");

    // 3.1.5 Неправильно зашифрованный текст - проверяем на русские буквы
    for(auto c : cipher_text) {
        if((c < L'А' || c > L'Я') && c != L'Ё') {
            throw cipher_error("Неправильно зашифрованный текст: содержит недопустимые символы");
        }
    }

    vector<int> work = convert(cipher_text);
    
    for(unsigned i = 0; i < work.size(); i++) {
        work[i] = (work[i] + alphaNum.size() - key[i % key.size()]) % alphaNum.size();
    }
    return convert(work);
}

vector<int> modAlphaCipher::convert(const wstring& s)
{
    vector<int> result;
    for(auto c : s) {
        result.push_back(alphaNum[c]);
    }
    return result;
}

wstring modAlphaCipher::convert(const vector<int>& v)
{
    wstring result;
    for(auto i : v) {
        result.push_back(numAlpha[i]);
    }
    return result;
}
