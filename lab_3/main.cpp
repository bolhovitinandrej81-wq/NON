#include <UnitTest++/UnitTest++.h>
#include "modAlphaCipher.h"
#include <locale>

using namespace std;

// Тесты конструктора (Таблица 1)
SUITE(ConstructorTests) {
    // Тест 1.1: Верный ключ
    TEST(ValidKey) {
        CHECK_NOTHROW(modAlphaCipher("МЫШКА"));
    }
    
    // Тест 1.2: Ключ с буквой Ё
    TEST(KeyWithYo) {
        CHECK_NOTHROW(modAlphaCipher("ПРОГРАММЁР"));
    }
    
    // Тест 1.3: Пустой ключ
    TEST(EmptyKey) {
        CHECK_THROW(modAlphaCipher(""), cipher_error);
    }
    
    // Тест 1.4: Ключ с пробелами
    TEST(KeyWithSpaces) {
        CHECK_THROW(modAlphaCipher("ВИРТУАЛЬНАЯ МАШИНА"), cipher_error);
    }
    
    // Тест 1.5: Ключ с цифрами
    TEST(KeyWithDigits) {
        CHECK_THROW(modAlphaCipher("КОД404"), cipher_error);
    }
    
    // Тест 1.6: Ключ с латинскими буквами
    TEST(KeyWithLatin) {
        CHECK_THROW(modAlphaCipher("MOUSE"), cipher_error);
    }
    
    // Тест 1.7: Ключ со спецсимволами
    TEST(KeyWithSpecialChars) {
        CHECK_THROW(modAlphaCipher("КЛАВИАТУРА!"), cipher_error);
    }
    
    // Тест 1.8: Ключ в нижнем регистре
    TEST(LowercaseKey) {
        CHECK_NOTHROW(modAlphaCipher("монитор"));
    }
    
    // Дополнительный тест: слабый ключ
    TEST(WeakKey) {
        CHECK_THROW(modAlphaCipher("ААА"), cipher_error);
    }
}

// Фикстура для тестов с ключом "МЫШКА"
struct KeyMyshkaFixture {
    modAlphaCipher* cipher;
    
    KeyMyshkaFixture() {
        cipher = new modAlphaCipher("МЫШКА");
    }
    
    ~KeyMyshkaFixture() {
        delete cipher;
    }
};

// Тесты метода encrypt (Таблица 2)
SUITE(EncryptTests) {
    // Тест 2.1: Текст без пробелов
    TEST_FIXTURE(KeyMyshkaFixture, TextWithoutSpaces) {
        string encrypted = cipher->encrypt("КОМПЬЮТЕР");
        CHECK(!encrypted.empty());
    }
    
    // Тест 2.2: Текст с пробелами
    TEST_FIXTURE(KeyMyshkaFixture, TextWithSpaces) {
        string encrypted = cipher->encrypt("НОВЫЙ КОМПЬЮТЕР");
        CHECK(!encrypted.empty());
    }
    
    // Тест 2.3: Текст в нижнем регистре
    TEST_FIXTURE(KeyMyshkaFixture, LowercaseText) {
        string encrypted = cipher->encrypt("программа");
        CHECK(!encrypted.empty());
    }
    
    // Тест 2.4: Текст с буквой Ё
    TEST_FIXTURE(KeyMyshkaFixture, TextWithYo) {
        string encrypted = cipher->encrypt("ЁМКОСТЬ");
        CHECK(!encrypted.empty());
    }
    
    // Тест 2.5: Пустой текст
    TEST_FIXTURE(KeyMyshkaFixture, EmptyText) {
        CHECK_THROW(cipher->encrypt(""), cipher_error);
    }
    
    // Тест 2.6: Текст с цифрами (ВЕРСИЯ2026 вместо 2024)
    TEST_FIXTURE(KeyMyshkaFixture, TextWithDigits) {
        // Согласно скриншоту, текст "ВЕРСИЯ2026" должен пройти шифрование
        string encrypted = cipher->encrypt("ВЕРСИЯ2026");
        CHECK(!encrypted.empty()); // Цифры игнорируются, текст шифруется
    }
    
    // Тест 2.7: Текст с латинскими буквами
    TEST_FIXTURE(KeyMyshkaFixture, TextWithLatin) {
        CHECK_THROW(cipher->encrypt("COMPUTER"), cipher_error);
    }
    
    // Тест 2.8: Текст со спецсимволами
    TEST_FIXTURE(KeyMyshkaFixture, TextWithSpecialChars) {
        string encrypted = cipher->encrypt("ПРИВЕТ.МИР!");
        CHECK(!encrypted.empty()); // Спецсимволы игнорируются
    }
    
    // Тест 2.9: Длинный текст (другой ключ)
    TEST(LongText) {
        modAlphaCipher cipher2("КОД");
        string encrypted = cipher2.encrypt("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ");
        CHECK(!encrypted.empty());
    }
    
    // Дополнительный тест: текст без русских букв
    TEST_FIXTURE(KeyMyshkaFixture, NoRussianText) {
        CHECK_THROW(cipher->encrypt("12345"), cipher_error);
    }
    
    // Дополнительный тест: максимальный ключ
    TEST(MaxShiftKey) {
        modAlphaCipher cipher3("Я");
        string encrypted = cipher3.encrypt("ХАМОН");
        CHECK(!encrypted.empty());
    }
}

// Фикстура для тестов с ключом "С" (как в примере)
struct KeyCFixture {
    modAlphaCipher* cipher;
    
    KeyCFixture() {
        cipher = new modAlphaCipher("С");
    }
    
    ~KeyCFixture() {
        delete cipher;
    }
};

// Тесты метода decrypt (Таблица 3)
SUITE(DecryptTests) {
    // Тест 3.1: Корректный зашифрованный текст
    TEST_FIXTURE(KeyMyshkaFixture, CorrectCipherText) {
        string original = "КОМПЬЮТЕР";
        string encrypted = cipher->encrypt(original);
        string decrypted = cipher->decrypt(encrypted);
        CHECK_EQUAL(original, decrypted);
    }
    
    // Тест 3.2: Текст с пробелами в оригинале
    TEST_FIXTURE(KeyMyshkaFixture, CipherTextWithSpacesInOriginal) {
        string original = "НОВЫЙ КОМПЬЮТЕР";
        string encrypted = cipher->encrypt(original);
        string decrypted = cipher->decrypt(encrypted);
        // Пробелы удаляются при шифровании
        CHECK_EQUAL("НОВЫЙКОМПЬЮТЕР", decrypted);
    }
    
    // Тест 3.3: Пустой шифротекст
    TEST_FIXTURE(KeyMyshkaFixture, EmptyCipherText) {
        CHECK_THROW(cipher->decrypt(""), cipher_error);
    }
    
    // Тест 3.4: Шифротекст в нижнем регистре
    TEST_FIXTURE(KeyMyshkaFixture, LowercaseCipherText) {
        CHECK_THROW(cipher->decrypt("зашифруй"), cipher_error);
    }
    
    // Тест 3.5: Шифротекст с цифрами
    TEST_FIXTURE(KeyMyshkaFixture, CipherTextWithDigits) {
        CHECK_THROW(cipher->decrypt("ТЕКСТ555"), cipher_error);
    }
    
    // Тест 3.6: Шифротекст с латинскими буквами
    TEST_FIXTURE(KeyMyshkaFixture, CipherTextWithLatin) {
        CHECK_THROW(cipher->decrypt("DECRYPT"), cipher_error);
    }
    
    // Тест 3.7: Шифротекст со спецсимволами
    TEST_FIXTURE(KeyMyshkaFixture, CipherTextWithSpecialChars) {
        CHECK_THROW(cipher->decrypt("СИМВОЛ@"), cipher_error);
    }
    
    // Тест 3.8: Полный цикл шифрование-дешифрование
    TEST_FIXTURE(KeyMyshkaFixture, FullCycle) {
        string original = "ПРОГРАММА";
        string encrypted = cipher->encrypt(original);
        string decrypted = cipher->decrypt(encrypted);
        CHECK_EQUAL(original, decrypted);
    }
    
    // Тест 3.9: Текст с буквой Ё
    TEST_FIXTURE(KeyMyshkaFixture, TextWithYoDecrypt) {
        string original = "ЁМКОСТЬ";
        string encrypted = cipher->encrypt(original);
        string decrypted = cipher->decrypt(encrypted);
        CHECK_EQUAL(original, decrypted);
    }
    
    // Тест с ключом "С" (как в примере)
    TEST_FIXTURE(KeyCFixture, DecryptWithKeyC) {
        string original = "ХАМОН";
        string encrypted = cipher->encrypt(original);
        string decrypted = cipher->decrypt(encrypted);
        CHECK_EQUAL(original, decrypted);
    }
    
    // Тест на максимальный ключ
    TEST(MaxKeyDecrypt) {
        modAlphaCipher cipher3("Я");
        string original = "АЛЫЙОВЕРДРАЙВ";
        string encrypted = cipher3.encrypt(original);
        string decrypted = cipher3.decrypt(encrypted);
        CHECK_EQUAL(original, decrypted);
    }
}

int main(int argc, char **argv) {
    // Устанавливаем локаль для корректной работы с русскими символами
    std::locale::global(std::locale("ru_RU.UTF-8"));
    
    // Запуск всех тестов
    return UnitTest::RunAllTests();
}
