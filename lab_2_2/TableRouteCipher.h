#pragma once
#include <string>
#include <stdexcept>

using namespace std;

class cipher_error : public invalid_argument {
public:
    explicit cipher_error(const string& what_arg) : invalid_argument(what_arg) {}
};

class TableRouteCipher {
private:
    int key;
public:
    TableRouteCipher() = delete;
    TableRouteCipher(int skey);
    string encrypt(string &text);
    string decrypt(string &text);
};
