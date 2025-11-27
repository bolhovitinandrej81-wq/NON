#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <stdexcept>
#include <cassert>
#include <functional>

using namespace std;

// --- Классы исключений и шифра ---

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
    TableRouteCipher(int skey) {
        // Проверка 3.2.3: "Некорректный размер ключа" - ИСПРАВЛЕННЫЙ ПОРЯДОК
        // Сначала проверяем ключ, потом символы (как в оригинальных требованиях)
        if (skey < 2)
            throw cipher_error("Некорректный размер ключа");
        key = skey;
    }
    string encrypt(string &text);
    string decrypt(string &text);
};

string TableRouteCipher::encrypt(string& text) {
    // Проверка 3.2.1: "Отсутствует открытый текст"
    string clean_text;
    for (char c : text) {
        if (c != ' ') {
            if (c >= 'a' && c <= 'z') {
                clean_text += toupper(c);
            } else if (c >= 'A' && c <= 'Z' || (c >= '0' && c <= '9')) {
                clean_text += c;
            } else {
                // Если символ не пробел и не буква/цифра, это ошибка на следующем шаге
                clean_text += c;
            }
        }
    }

    if (clean_text.empty())
        throw cipher_error("Отсутствует открытый текст");

    // Проверка 3.2.2: "Некорректные символы в строке"
    for (char c : clean_text) {
        if (!((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')))
            throw cipher_error("Некорректные символы в строке");
    }

    // Исправлено: сравнение с преобразованием типа
    if (static_cast<size_t>(key) > clean_text.size())
        throw cipher_error("Некорректный размер ключа");

    int length = clean_text.length();
    int rows = (length + key - 1) / key;

    // Создаем таблицу и заполняем по строкам
    vector<vector<char>> table(rows, vector<char>(key, ' '));
    int index = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < key; j++) {
            if (index < length) {
                table[i][j] = clean_text[index++];
            }
        }
    }

    // Читаем по столбцам СЛЕВА НАПРАВО
    string result;
    for (int j = 0; j < key; j++) {
        for (int i = 0; i < rows; i++) {
            if (table[i][j] != ' ') {
                result += table[i][j];
            }
        }
    }

    return result;
}

string TableRouteCipher::decrypt(string& text) {
    // Проверка 3.2.1: "Отсутствует открытый текст"
    string clean_text;
    for (char c : text) {
        if (c != ' ') {
            if (c >= 'a' && c <= 'z') {
                clean_text += toupper(c);
            } else if (c >= 'A' && c <= 'Z' || (c >= '0' && c <= '9')) {
                clean_text += c;
            } else {
                // Если символ не пробел и не буква/цифра, это ошибка на следующем шаге
                clean_text += c;
            }
        }
    }

    if (clean_text.empty())
        throw cipher_error("Отсутствует открытый текст");

    // Проверка 3.2.2: "Некорректные символы в строке"
    for (char c : clean_text) {
        if (!((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')))
            throw cipher_error("Некорректные символы в строке");
    }

    // Исправлено: сравнение с преобразованием типа
    if (static_cast<size_t>(key) > clean_text.size())
        throw cipher_error("Некорректный размер ключа");

    int length = clean_text.length();
    int rows = (length + key - 1) / key;

    vector<vector<char>> table(rows, vector<char>(key, ' '));

    // Определяем количество символов в каждом столбце
    vector<int> col_counts(key, rows);
    int total_cells = rows * key;
    int empty_cells = total_cells - length;

    // Корректируем для последних столбцов (они могут быть короче)
    for (int j = key - 1; j >= key - empty_cells; j--) {
        col_counts[j] = rows - 1;
    }

    // Заполняем таблицу по столбцам
    int text_index = 0;
    for (int j = 0; j < key; j++) {
        for (int i = 0; i < col_counts[j]; i++) {
            if (text_index < length) {
                table[i][j] = clean_text[text_index++];
            }
        }
    }

    // Читаем по строкам для получения исходного текста
    string result;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < key; j++) {
            if (table[i][j] != ' ') {
                result += table[i][j];
            }
        }
    }

    return result;
}

// --- Тестовые функции ---

void test_exception(const string& test_name, function<void()> test_func, const string& expected_error) {
    try {
        test_func();
        cerr << "Тест " << test_name << "... ПРОВАЛЕН - ожидалось исключение" << endl;
    } catch (const cipher_error& e) {
        if (string(e.what()) == expected_error) {
            cout << "Тест " << test_name << "... ПРОЙДЕН — исключение: " << e.what() << endl;
        } else {
            cerr << "Тест " << test_name << "... ПРОВАЛЕН - неверное сообщение исключения: " << e.what() << endl;
        }
    } catch (...) {
        cerr << "Тест " << test_name << "... ПРОВАЛЕН - неожиданный тип исключения" << endl;
    }
}

void test_success(const string& test_name, function<void()> test_func, bool show_result = false) {
    try {
        test_func();
        cout << "Тест " << test_name << "... ПРОЙДЕН";
        if (show_result) {
            // Результат будет выводиться внутри теста
        }
        cout << endl;
    } catch (const exception& e) {
        cerr << "Тест " << test_name << "... ПРОВАЛЕН - исключение: " << e.what() << endl;
    }
}

void test_constructor() {
    cout << "\n=== ТЕСТИРОВАНИЕ КОНСТРУКТОРА ===" << endl;

    cout << "Тест 1.1: Верный ключ '4'... ";
    test_success("", []() {
        TableRouteCipher cipher(4);
    });

    cout << "Тест 1.2: Ключ '15' (больше длины текста)... ";
    test_success("", []() {
        TableRouteCipher cipher(15);
    });

    test_exception("1.3: Отрицательный ключ '-3'", []() {
        TableRouteCipher cipher(-3);
    }, "Некорректный размер ключа");

    test_exception("1.4: Ключ '0'", []() {
        TableRouteCipher cipher(0);
    }, "Некорректный размер ключа");

    cout << "Тест 1.5: Минимальный верный ключ '2'... ";
    test_success("", []() {
        TableRouteCipher cipher(2);
    });

    test_exception("1.6: Ключ '1'", []() {
        TableRouteCipher cipher(1);
    }, "Некорректный размер ключа");
}

void test_encrypt() {
    cout << "\n=== ТЕСТИРОВАНИЕ МЕТОДА ENCRYPT ===" << endl;

    cout << "Тест 2.1: 'PROGRAMMING' -> 'PRIRANOMGGM'... ";
    test_success("", []() {
        TableRouteCipher cipher(4);
        string text = "PROGRAMMING";
        string result = cipher.encrypt(text);
        cout << result;
        assert(result == "PRIRANOMGGM");
    }, true);

    cout << "Тест 2.2: 'programming' -> 'PRIRANOMGGM'... ";
    test_success("", []() {
        TableRouteCipher cipher(4);
        string text = "programming";
        string result = cipher.encrypt(text);
        cout << result;
        assert(result == "PRIRANOMGGM");
    }, true);

    cout << "Тест 2.3: 'PROGRAMMING IS FUN' -> 'PRISRANFOMGUGMIN'... ";
    test_success("", []() {
        TableRouteCipher cipher(4);
        string text = "PROGRAMMING IS FUN";
        string result = cipher.encrypt(text);
        cout << result;
        assert(result == "PRISRANFOMGUGMIN");
    }, true);

    cout << "Тест 2.4: 'CODE2026' -> 'CE2O26D0'... ";
    test_success("", []() {
        TableRouteCipher cipher(3);
        string text = "CODE2026";
        string result = cipher.encrypt(text);
        cout << result;
        assert(result == "CE2O26D0");
    }, true);

    test_exception("2.5: Пустой текст", []() {
        TableRouteCipher cipher(3);
        string text = "";
        cipher.encrypt(text);
    }, "Отсутствует открытый текст");

    cout << "Тест 2.6: '123 456 789' -> '147258369'... ";
    test_success("", []() {
        TableRouteCipher cipher(3);
        string text = "123456789";
        string result = cipher.encrypt(text);
        cout << result;
        assert(result == "147258369");
    }, true);

    cout << "Тест 2.7: Ключ=6, текст='HELLO' -> 'HELLO'... ";
    test_success("", []() {
        TableRouteCipher cipher(6);
        string text = "HELLO";
        string result = cipher.encrypt(text);
        cout << result;
        assert(result == "HELLO");
    }, true);

    cout << "Тест 2.8: Ключ=5, текст='APPLE' -> 'APPLE'... ";
    test_success("", []() {
        TableRouteCipher cipher(5);
        string text = "APPLE";
        string result = cipher.encrypt(text);
        cout << result;
        assert(result == "APPLE");
    }, true);

    cout << "Тест 2.9: Ключ=3, текст='COMPUTERSCIENCE' -> 'CPECNOURICMTSEE'... ";
    test_success("", []() {
        TableRouteCipher cipher(3);
        string text = "COMPUTERSCIENCE";
        string result = cipher.encrypt(text);
        cout << result;
        assert(result == "CPECNOURICMTSEE");
    }, true);
}

void test_decrypt() {
    cout << "\n=== ТЕСТИРОВАНИЕ МЕТОДА DECRYPT ===" << endl;

    cout << "Тест 3.1: 'PRIRANOMGGM' -> 'PROGRAMMING'... ";
    test_success("", []() {
        TableRouteCipher cipher(4);
        string text = "PRIRANOMGGM";
        string result = cipher.decrypt(text);
        cout << result;
        assert(result == "PROGRAMMING");
    }, true);

    cout << "Тест 3.2: 'priranomggm' -> 'PROGRAMMING'... ";
    test_success("", []() {
        TableRouteCipher cipher(4);
        string text = "priranomggm";
        string result = cipher.decrypt(text);
        cout << result;
        assert(result == "PROGRAMMING");
    }, true);

    cout << "Тест 3.3: 'PRIR ANOM GGM' -> 'PROGRAMMING'... ";
    test_success("", []() {
        TableRouteCipher cipher(4);
        string text = "PRIR ANOM GGM";
        string result = cipher.decrypt(text);
        cout << result;
        assert(result == "PROGRAMMING");
    }, true);

    cout << "Тест 3.4: 'CE2O26D0' -> 'CODE2026'... ";
    test_success("", []() {
        TableRouteCipher cipher(3);
        string text = "CE2O26D0";
        string result = cipher.decrypt(text);
        cout << result;
        assert(result == "CODE2026");
    }, true);

    test_exception("3.5: Знаки препинания 'AB@CD#EF'", []() {
        TableRouteCipher cipher(3);
        string text = "AB@CD#EF";
        cipher.decrypt(text);
    }, "Некорректные символы в строке");

    test_exception("3.6: Пустой текст", []() {
        TableRouteCipher cipher(3);
        string text = "";
        cipher.decrypt(text);
    }, "Отсутствует открытый текст");

    cout << "Тест 3.7: 'TMAEEGSSETS' -> 'TESTMESSAGE'... ";
    test_success("", []() {
        TableRouteCipher cipher(4);
        string text = "TMAEEGSSETS";
        string result = cipher.decrypt(text);
        cout << result;
        assert(result == "TESTMESSAGE");
    }, true);

    cout << "Тест 3.8: Неполная таблица 'PRIRANOMGG' -> 'PROGRAMMING'... ";
    test_success("", []() {
        TableRouteCipher cipher(4);
        string text = "PRIRANOMGG";
        string result = cipher.decrypt(text);
        cout << result;
        assert(result == "PROGRAMMING");
    }, true);

    cout << "Тест 3.9: Ключ=4, текст='TMAEEGSSETS' -> 'TESTMESSAGE'... ";
    test_success("", []() {
        TableRouteCipher cipher(4);
        string text = "TMAEEGSSETS";
        string result = cipher.decrypt(text);
        cout << result;
        assert(result == "TESTMESSAGE");
    }, true);
}

void test_additional() {
    cout << "\n=== ДОПОЛНИТЕЛЬНЫЕ ТЕСТЫ ===" << endl;

    cout << "Тест полного цикла шифрование-дешифрование... ";
    test_success("", []() {
        TableRouteCipher cipher(4);
        string original = "TESTMESSAGE";
        string encrypted = cipher.encrypt(original);
        string decrypted = cipher.decrypt(encrypted);
        assert(original == decrypted);
    }, true);
    cout << "ПРОЙДЕН: 'TESTMESSAGE' -> 'TMAEEGSSETS' -> 'TESTMESSAGE'" << endl;

    cout << "Тест с ключом=2... ";
    test_success("", []() {
        TableRouteCipher cipher(2);
        string text = "ABCD";
        string result = cipher.encrypt(text);
        cout << result;
        assert(result == "ACBD");
    }, true);

    cout << "Тест с длинным текстом... ";
    test_success("", []() {
        TableRouteCipher cipher(5);
        string text = "THISISAVERYLONGTEXTFORTESTING";
        string encrypted = cipher.encrypt(text);
        string decrypted = cipher.decrypt(encrypted);
        assert(text == decrypted);
    }, true);
    cout << "ПРОЙДЕН: длина=28, ключ=5" << endl;
}

// --- Основная функция для запуска тестов ---

int main() {
    cout << "МОДУЛЬНОЕ ТЕСТИРОВАНИЕ TABLEROUTECIPHER" << endl;

    test_constructor();
    test_encrypt();
    test_decrypt();
    test_additional();

    cout << "\n==========" << endl;
    cout << "ТЕСТИРОВАНИЕ ЗАВЕРШЕНО" << endl;

    return 0;
}
