#include "modAlphaCipher.h"
#include <algorithm>
#include <stdexcept>

using namespace std;

modAlphaCipher::modAlphaCipher(const string& skey)
{
    // Инициализируем алфавит и маппинг
    for(unsigned i = 0; i < numAlpha.size(); i++) {
        alphaNum[numAlpha[i]] = i;
    }
    key = convert(skey);
}

string modAlphaCipher::encrypt(const string& open_text)
{
    vector<int> work = convert(open_text);
    for(unsigned i = 0; i < work.size(); i++) {
        work[i] = (work[i] + key[i % key.size()]) % alphaNum.size();
    }
    return convert(work);
}

string modAlphaCipher::decrypt(const string& cipher_text)
{
    vector<int> work = convert(cipher_text);
    for(unsigned i = 0; i < work.size(); i++) {
        work[i] = (work[i] + alphaNum.size() - key[i % key.size()]) % alphaNum.size();
    }
    return convert(work);
}

vector<int> modAlphaCipher::convert(const string& s)
{
    vector<int> result;
    for(auto c : s) {
        result.push_back(alphaNum[c]);
    }
    return result;
}

string modAlphaCipher::convert(const vector<int>& v)
{
    string result;
    for(auto i : v) {
        result.push_back(numAlpha[i]);
    }
    return result;
}
