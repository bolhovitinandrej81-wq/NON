#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
using namespace std;

class cipher_error: public invalid_argument {
    public:
    explicit cipher_error(const string& what_arg):
    invalid_argument(what_arg) {}
    explicit cipher_error(const char* what_arg):
    invalid_argument(what_arg) {}
};

class TableRouteCipher {
    private:
    int key;
    inline int getValidKey(int key, const string& Text);
    inline string getValidOpenText(const string& s);
    inline string getValidCipherText(const string& s);
    
    public:
    TableRouteCipher() = delete;
    TableRouteCipher(int k);
    
    string encrypt(const string& text);
    string decrypt(const string& text);
};
