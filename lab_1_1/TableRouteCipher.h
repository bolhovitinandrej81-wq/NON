#pragma once
#include <string>

using namespace std;

class TableRouteCipher {
private:
    int key;
public:
    TableRouteCipher() = delete;
    TableRouteCipher(int skey);
    string encrypt(string &text);
    string decrypt(string &text);
};
