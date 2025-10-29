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
        work[i] = (work[i] + key[i % key.size()]) % alphaNum.size();
    }
    return convert(work);
}

wstring modAlphaCipher::decrypt(const wstring& cipher_text)
{
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
        // Пробелы больше не обрабатываем, так как они удаляются до шифрования
        result.push_back(alphaNum[c]);
    }
    return result;
}

wstring modAlphaCipher::convert(const vector<int>& v)
{
    wstring result;
    for(auto i : v) {
        // Пробелы больше не восстанавливаем
        result.push_back(numAlpha[i]);
    }
    return result;
}
