#include "modAlphaCipher.h"
#include <locale>
#include <codecvt>
#include <cwctype>
#include <algorithm>

using namespace std;

modAlphaCipher::modAlphaCipher(const wstring& skey)
{
    for(unsigned i = 0; i < numAlpha.size(); i++) {
        alphaNum[numAlpha[i]] = i;
    }
    key = convert(skey);
}

wstring modAlphaCipher::encrypt(const wstring& open_text)
{
    vector<int> work = convert(open_text);
    for(unsigned i = 0; i < work.size(); i++) {
        // Пропускаем пробелы (они имеют значение -1)
        if(work[i] != -1) {
            work[i] = (work[i] + key[i % key.size()]) % alphaNum.size();
        }
    }
    return convert(work);
}

wstring modAlphaCipher::decrypt(const wstring& cipher_text)
{
    vector<int> work = convert(cipher_text);
    for(unsigned i = 0; i < work.size(); i++) {
        // Пропускаем пробелы (они имеют значение -1)
        if(work[i] != -1) {
            work[i] = (work[i] + alphaNum.size() - key[i % key.size()]) % alphaNum.size();
        }
    }
    return convert(work);
}

vector<int> modAlphaCipher::convert(const wstring& s)
{
    vector<int> result;
    for(auto c : s) {
        if(c == L' ') {
            // Пробелы помечаем как -1
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
            // Восстанавливаем пробелы
            result.push_back(L' ');
        } else {
            result.push_back(numAlpha[i]);
        }
    }
    return result;
}
