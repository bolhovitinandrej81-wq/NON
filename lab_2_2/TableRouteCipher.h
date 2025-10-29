#pragma once
#include <string>

using namespace std;

class cipher_error : public std::exception {
private:
    string message;
public:
    explicit cipher_error(const string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
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
