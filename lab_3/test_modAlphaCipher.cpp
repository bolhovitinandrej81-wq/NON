#include <iostream>
#include <locale>
#include <codecvt>
#include <map>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

using namespace std;

// Класс исключения
class cipher_error : public std::runtime_error {
public:
    explicit cipher_error(const std::string& what_arg) : std::runtime_error(what_arg) {}
};

// Класс шифра
class modAlphaCipher
{
private:
    std::wstring numAlpha = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    std::map<wchar_t, int> alphaNum;
    std::vector<int> key;
    std::vector<int> convert(const std::wstring& s);
    std::wstring convert(const std::vector<int>& v);
    
public:
    modAlphaCipher() = delete;
    modAlphaCipher(const std::wstring& skey);
    std::wstring encrypt(const std::wstring& open_text);
    std::wstring decrypt(const std::wstring& cipher_text);
};

// Реализация методов
modAlphaCipher::modAlphaCipher(const std::wstring& skey)
{
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

std::wstring modAlphaCipher::encrypt(const std::wstring& open_text)
{
    if(open_text.empty())
        throw cipher_error("Отсутствует текст");

    vector<int> work = convert(open_text);
    
    for(unsigned i = 0; i < work.size(); i++) {
        work[i] = (work[i] + key[i % key.size()]) % alphaNum.size();
    }
    return convert(work);
}

std::wstring modAlphaCipher::decrypt(const std::wstring& cipher_text)
{
    if(cipher_text.empty())
        throw cipher_error("Пустой текст");

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

vector<int> modAlphaCipher::convert(const std::wstring& s)
{
    vector<int> result;
    for(auto c : s) {
        result.push_back(alphaNum[c]);
    }
    return result;
}

std::wstring modAlphaCipher::convert(const std::vector<int>& v)
{
    wstring result;
    for(auto i : v) {
        result.push_back(numAlpha[i]);
    }
    return result;
}

// Вспомогательные функции
wstring to_wide(const string& narrow_str) {
    try {
        wstring_convert<codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(narrow_str);
    } catch (const exception& e) {
        return L"";
    }
}

string to_narrow(const wstring& wide_str) {
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wide_str);
}

wstring to_upper_rus(const wstring& s) {
    wstring result = s;
    for (auto& c : result) {
        if (c >= L'а' && c <= L'я') {
            c = c - L'а' + L'А';
        } else if (c == L'ё') {
            c = L'Ё';
        }
    }
    return result;
}

wstring remove_spaces(const wstring& s) {
    wstring result;
    for(auto c : s) {
        if(c != L' ') {
            result += c;
        }
    }
    return result;
}

// ТЕСТЫ ДЛЯ КОНСТРУКТОРА (Таблица 1)
void test_constructor() {
    cout << "=== ТЕСТИРОВАНИЕ КОНСТРУКТОРА ===" << endl;
    
    // Тест 1.1: Верный ключ
    try {
        modAlphaCipher cipher1(to_wide("МЫШКА"));
        cout << "Тест 1.1: Верный ключ 'МЫШКА'... ПРОЙДЕН" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 1.1: Верный ключ 'МЫШКА'... ПРОВАЛЕН - " << e.what() << endl;
    }
    
    // Тест 1.2: Ключ с буквой Ё
    try {
        modAlphaCipher cipher2(to_wide("ПРОГРАММЁР"));
        cout << "Тест 1.2: Ключ с буквой Ё 'ПРОГРАММЁР'... ПРОЙДЕН" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 1.2: Ключ с буквой Ё 'ПРОГРАММЁР'... ПРОВАЛЕН - " << e.what() << endl;
    }
    
    // Тест 1.3: Пустой ключ
    try {
        modAlphaCipher cipher3(L"");
        cout << "Тест 1.3: Пустой ключ... ПРОВАЛЕН" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 1.3: Пустой ключ... ПРОЙДЕН - " << e.what() << endl;
    }
    
    // Тест 1.4: Ключ с пробелами
    try {
        modAlphaCipher cipher4(to_wide("ВИРТУАЛЬНАЯ МАШИНА"));
        cout << "Тест 1.4: Ключ с пробелами 'ВИРТУАЛЬНАЯ МАШИНА'... ПРОВАЛЕН" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 1.4: Ключ с пробелами 'ВИРТУАЛЬНАЯ МАШИНА'... ПРОЙДЕН - " << e.what() << endl;
    }
    
    // Тест 1.5: Ключ с цифрами
    try {
        modAlphaCipher cipher5(to_wide("КОД404"));
        cout << "Тест 1.5: Ключ с цифрами 'КОД404'... ПРОВАЛЕН" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 1.5: Ключ с цифрами 'КОД404'... ПРОЙДЕН - " << e.what() << endl;
    }
    
    // Тест 1.6: Ключ с латинскими буквами
    try {
        modAlphaCipher cipher6(to_wide("MOUSE"));
        cout << "Тест 1.6: Ключ с латинскими буквами 'MOUSE'... ПРОВАЛЕН" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 1.6: Ключ с латинскими буквами 'MOUSE'... ПРОЙДЕН - " << e.what() << endl;
    }
    
    // Тест 1.7: Ключ со спецсимволами
    try {
        modAlphaCipher cipher7(to_wide("КЛАВИАТУРА!"));
        cout << "Тест 1.7: Ключ со спецсимволами 'КЛАВИАТУРА!'... ПРОВАЛЕН" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 1.7: Ключ со спецсимволами 'КЛАВИАТУРА!'... ПРОЙДЕН - " << e.what() << endl;
    }
    
    // Тест 1.8: Ключ в нижнем регистре
    try {
        wstring key_lower = to_wide("монитор");
        key_lower = to_upper_rus(key_lower);
        modAlphaCipher cipher8(key_lower);
        cout << "Тест 1.8: Ключ в нижнем регистре 'монитор'... ПРОЙДЕН" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 1.8: Ключ в нижнем регистре 'монитор'... ПРОВАЛЕН - " << e.what() << endl;
    }
}

// ТЕСТЫ ДЛЯ МЕТОДА ENCRYPT (Таблица 2)
void test_encrypt() {
    cout << "\n=== ТЕСТИРОВАНИЕ МЕТОДА ENCRYPT ===" << endl;
    
    modAlphaCipher cipher(to_wide("МЫШКА"));
    
    // Тест 2.1: Текст без пробелов
    try {
        wstring encrypted = cipher.encrypt(to_wide("КОМПЬЮТЕР"));
        cout << "Тест 2.1: Текст без пробелов 'КОМПЬЮТЕР'... ПРОЙДЕН: '" << to_narrow(encrypted) << "'" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 2.1: Текст без пробелов 'КОМПЬЮТЕР'... ПРОВАЛЕН - " << e.what() << endl;
    }
    
    // Тест 2.2: Текст с пробелами
    try {
        wstring text = to_wide("НОВЫЙ КОМПЬЮТЕР");
        wstring text_no_spaces = remove_spaces(text);
        wstring encrypted = cipher.encrypt(text_no_spaces);
        cout << "Тест 2.2: Текст с пробелами 'НОВЫЙ КОМПЬЮТЕР'... ПРОЙДЕН: '" << to_narrow(encrypted) << "'" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 2.2: Текст с пробелами 'НОВЫЙ КОМПЬЮТЕР'... ПРОВАЛЕН - " << e.what() << endl;
    }
    
    // Тест 2.3: Текст в нижнем регистре
    try {
        wstring text_lower = to_wide("программа");
        text_lower = to_upper_rus(text_lower);
        wstring encrypted = cipher.encrypt(text_lower);
        cout << "Тест 2.3: Текст в нижнем регистре 'программа'... ПРОЙДЕН: '" << to_narrow(encrypted) << "'" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 2.3: Текст в нижнем регистре 'программа'... ПРОВАЛЕН - " << e.what() << endl;
    }
    
    // Тест 2.4: Текст с буквой Ё
    try {
        wstring encrypted = cipher.encrypt(to_wide("ЁМКОСТЬ"));
        cout << "Тест 2.4: Текст с буквой Ё 'ЁМКОСТЬ'... ПРОЙДЕН: '" << to_narrow(encrypted) << "'" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 2.4: Текст с буквой Ё 'ЁМКОСТЬ'... ПРОВАЛЕН - " << e.what() << endl;
    }
    
    // Тест 2.5: Пустой текст
    try {
        wstring encrypted = cipher.encrypt(L"");
        cout << "Тест 2.5: Пустой текст... ПРОВАЛЕН" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 2.5: Пустой текст... ПРОЙДЕН - " << e.what() << endl;
    }
    
    // Тест 2.6: Текст с цифрами
    try {
        wstring encrypted = cipher.encrypt(to_wide("ВЕРСИЯ2024"));
        cout << "Тест 2.6: Текст с цифрами 'ВЕРСИЯ2024'... ПРОВАЛЕН" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 2.6: Текст с цифрами 'ВЕРСИЯ2024'... ПРОЙДЕН - " << e.what() << endl;
    }
    
    // Тест 2.7: Текст с латинскими буквами
    try {
        wstring encrypted = cipher.encrypt(to_wide("COMPUTER"));
        cout << "Тест 2.7: Текст с латинскими буквами 'COMPUTER'... ПРОВАЛЕН" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 2.7: Текст с латинскими буквами 'COMPUTER'... ПРОЙДЕН - " << e.what() << endl;
    }
    
    // Тест 2.8: Текст со спецсимволами
    try {
        wstring encrypted = cipher.encrypt(to_wide("ПРИВЕТ,МИР!"));
        cout << "Тест 2.8: Текст со спецсимволами 'ПРИВЕТ,МИР!'... ПРОВАЛЕН" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 2.8: Текст со спецсимволами 'ПРИВЕТ,МИР!'... ПРОЙДЕН - " << e.what() << endl;
    }
    
    // Тест 2.9: Длинный текст
    try {
        modAlphaCipher cipher2(to_wide("КОД"));
        wstring long_text = to_wide("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ");
        wstring encrypted = cipher2.encrypt(long_text);
        cout << "Тест 2.9: Длинный текст... ПРОЙДЕН: '" << to_narrow(encrypted).substr(0, 20) << "...'" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 2.9: Длинный текст... ПРОВАЛЕН - " << e.what() << endl;
    }
}

// ТЕСТЫ ДЛЯ МЕТОДА DECRYPT (Таблица 3)
void test_decrypt() {
    cout << "\n=== ТЕСТИРОВАНИЕ МЕТОДА DECRYPT ===" << endl;
    
    modAlphaCipher cipher(to_wide("МЫШКА"));
    
    // Тест 3.1: Корректный зашифрованный текст
    try {
        wstring original = to_wide("КОМПЬЮТЕР");
        wstring encrypted = cipher.encrypt(original);
        wstring decrypted = cipher.decrypt(encrypted);
        
        if (decrypted == original) {
            cout << "Тест 3.1: Корректный зашифрованный текст... ПРОЙДЕН" << endl;
        } else {
            cout << "Тест 3.1: Корректный зашифрованный текст... ПРОВАЛЕН" << endl;
        }
    } catch (const cipher_error& e) {
        cout << "Тест 3.1: Корректный зашифрованный текст... ПРОВАЛЕН - " << e.what() << endl;
    }
    
    // Тест 3.2: Текст с пробелами
    try {
        wstring original = to_wide("НОВЫЙ КОМПЬЮТЕР");
        wstring original_no_spaces = remove_spaces(original);
        wstring encrypted = cipher.encrypt(original_no_spaces);
        wstring decrypted = cipher.decrypt(encrypted);
        
        if (decrypted == original_no_spaces) {
            cout << "Тест 3.2: Текст с пробелами... ПРОЙДЕН" << endl;
        } else {
            cout << "Тест 3.2: Текст с пробелами... ПРОВАЛЕН" << endl;
        }
    } catch (const cipher_error& e) {
        cout << "Тест 3.2: Текст с пробелами... ПРОВАЛЕН - " << e.what() << endl;
    }
    
    // Тест 3.3: Пустой текст
    try {
        wstring decrypted = cipher.decrypt(L"");
        cout << "Тест 3.3: Пустой текст... ПРОВАЛЕН" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 3.3: Пустой текст... ПРОЙДЕН - " << e.what() << endl;
    }
    
    // Тест 3.4: Текст в нижнем регистре
    try {
        wstring decrypted = cipher.decrypt(to_wide("зашифруй"));
        cout << "Тест 3.4: Текст в нижнем регистре... ПРОВАЛЕН" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 3.4: Текст в нижнем регистре... ПРОЙДЕН - " << e.what() << endl;
    }
    
    // Тест 3.5: Текст с цифрами
    try {
        wstring decrypted = cipher.decrypt(to_wide("ТЕКСТ555"));
        cout << "Тест 3.5: Текст с цифрами 'ТЕКСТ555'... ПРОВАЛЕН" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 3.5: Текст с цифрами 'ТЕКСТ555'... ПРОЙДЕН - " << e.what() << endl;
    }
    
    // Тест 3.6: Текст с латинскими буквами
    try {
        wstring decrypted = cipher.decrypt(to_wide("DECRYPT"));
        cout << "Тест 3.6: Текст с латинскими буквами 'DECRYPT'... ПРОВАЛЕН" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 3.6: Текст с латинскими буквами 'DECRYPT'... ПРОЙДЕН - " << e.what() << endl;
    }
    
    // Тест 3.7: Текст со спецсимволами
    try {
        wstring decrypted = cipher.decrypt(to_wide("СИМВОЛ@"));
        cout << "Тест 3.7: Текст со спецсимволами 'СИМВОЛ@'... ПРОВАЛЕН" << endl;
    } catch (const cipher_error& e) {
        cout << "Тест 3.7: Текст со спецсимволами 'СИМВОЛ@'... ПРОЙДЕН - " << e.what() << endl;
    }
    
    // Тест 3.8: Полный цикл шифрование-дешифрование
    try {
        wstring original = to_wide("ПРОГРАММА");
        wstring encrypted = cipher.encrypt(original);
        wstring decrypted = cipher.decrypt(encrypted);
        
        if (decrypted == original) {
            cout << "Тест 3.8: Полный цикл шифрование-дешифрование... ПРОЙДЕН" << endl;
        } else {
            cout << "Тест 3.8: Полный цикл шифрование-дешифрование... ПРОВАЛЕН" << endl;
        }
    } catch (const cipher_error& e) {
        cout << "Тест 3.8: Полный цикл шифрование-дешифрование... ПРОВАЛЕН - " << e.what() << endl;
    }
    
    // Тест 3.9: Текст с буквой Ё
    try {
        wstring original = to_wide("ЁМКОСТЬ");
        wstring encrypted = cipher.encrypt(original);
        wstring decrypted = cipher.decrypt(encrypted);
        
        if (decrypted == original) {
            cout << "Тест 3.9: Текст с буквой Ё... ПРОЙДЕН" << endl;
        } else {
            cout << "Тест 3.9: Текст с буквой Ё... ПРОВАЛЕН" << endl;
        }
    } catch (const cipher_error& e) {
        cout << "Тест 3.9: Текст с буквой Ё... ПРОВАЛЕН - " << e.what() << endl;
    }
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    cout << "МОДУЛЬНОЕ ТЕСТИРОВАНИЕ MODALPHACIPHER" << endl;
    cout << "=========================================" << endl;
    
    test_constructor();  // Тесты из Таблицы 1
    test_encrypt();      // Тесты из Таблицы 2  
    test_decrypt();      // Тесты из Таблицы 3
    
    cout << "\n=========================================" << endl;
    cout << "ТЕСТИРОВАНИЕ ЗАВЕРШЕНО" << endl;
    
    return 0;
}
