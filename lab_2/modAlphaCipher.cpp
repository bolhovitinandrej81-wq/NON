#include "modAlphaCipher.h"
#include <algorithm>

using namespace std;

modAlphaCipher::modAlphaCipher(const wstring& skey)
{
    // Эти проверки уже сделаны в main, но оставляем для безопасности
    if(skey.empty())
        throw cipher_error("Пустой ключ");
    
    for(auto c : skey) {
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
    // Проверка уже в main, но оставляем
    if(open_text.empty())
        throw cipher_error("Отсутствует текст");
    
    vector<int> work = convert(open_text);
    int key_index = 0;
    
    for(unsigned i = 0; i < work.size(); i++) {
        if(work[i] != -1) {
            work[i] = (work[i] + key[key_index % key.size()]) % alphaNum.size();
            key_index++;
        }
    }
    return convert(work);
}

wstring modAlphaCipher::decrypt(const wstring& cipher_text)
{
    // Проверка пустого текста уже в main
    if(cipher_text.empty())
        throw cipher_error("Пустой текст");
    
    // Проверка символов уже в main, но оставляем для безопасности
    for(auto c : cipher_text) {
        if(c != L' ') {
            if((c < L'А' || c > L'Я') && c != L'Ё') {
                throw cipher_error("Неправильно зашифрованный текст: содержит недопустимые символы");
            }
        }
    }
    
    vector<int> work = convert(cipher_text);
    int key_index = 0;
    
    for(unsigned i = 0; i < work.size(); i++) {
        if(work[i] != -1) {
            work[i] = (work[i] + alphaNum.size() - key[key_index % key.size()]) % alphaNum.size();
            key_index++;
        }
    }
    return convert(work);
}

vector<int> modAlphaCipher::convert(const wstring& s)
{
    vector<int> result;
    for(auto c : s) {
        if(c == L' ') {
            result.push_back(-1);
        } else {
            result.push_back(alphaNum[c]);
        }
    }
    return result;
}

wstring modAlphaCipher::convert(const vector<int>& v)
{
    wstring result;
    for(auto i : v) {
        if(i == -1) {
            result.push_back(L' ');
        } else {
            result.push_back(numAlpha[i]);
        }
    }
    return result;
}
