#include "TableRouteCipher.h"
#include <UnitTest++/UnitTest++.h>
#include <string>
#include <iostream>

using namespace std;

// ========= ТЕСТЫ КОНСТРУКТОРА =========
SUITE(ConstructorTest) {
    // Позитивные тесты - заменим CHECK_NO_THROW
    TEST(ValidKey) {
        // Вместо CHECK_NO_THROW используем try-catch
        try {
            TableRouteCipher cipher(3);
            CHECK(true); // Если не было исключения, тест пройден
        } catch (const cipher_error&) {
            CHECK(false); // Если было исключение, тест не пройден
        }
    }
    
    TEST(KeyGreaterThanTextLength) {
        try {
            TableRouteCipher cipher(10);
            CHECK(true);
        } catch (const cipher_error&) {
            CHECK(false);
        }
    }
    
    TEST(KeyEqualsOne) {
        try {
            TableRouteCipher cipher(1);
            CHECK(true);
        } catch (const cipher_error&) {
            CHECK(false);
        }
    }
    
    // Негативные тесты - CHECK_THROW должен работать
    TEST(NegativeKey) {
        CHECK_THROW(TableRouteCipher(-5), cipher_error);
    }
    
    TEST(ZeroKey) {
        CHECK_THROW(TableRouteCipher(0), cipher_error);
    }
    
    TEST(KeyLessThanOne) {
        CHECK_THROW(TableRouteCipher(-10), cipher_error);
    }
}

// ========= ФИКСТУРА ДЛЯ КЛЮЧА 3 =========
struct Key3Fixture {
    TableRouteCipher* p;
    Key3Fixture() {
        p = new TableRouteCipher(3);
    }
    ~Key3Fixture() {
        delete p;
    }
};

// ========= ФИКСТУРА ДЛЯ КЛЮЧА 5 =========
struct Key5Fixture {
    TableRouteCipher* p;
    Key5Fixture() {
        p = new TableRouteCipher(5);
    }
    ~Key5Fixture() {
        delete p;
    }
};

// ========= ТЕСТЫ МЕТОДА ENCRYPT =========
SUITE(EncryptTest) {
    // Тесты с фикстурой Key3Fixture (ключ 3)
    TEST_FIXTURE(Key3Fixture, UpCaseString) {
        CHECK_EQUAL("PGMNRRMGOAI", p->encrypt("PROGRAMMING"));
    }
    
    TEST_FIXTURE(Key3Fixture, LowCaseString) {
        CHECK_EQUAL("pgmnrrmgoai", p->encrypt("programming"));
    }
    
    TEST_FIXTURE(Key3Fixture, StringWithWhitespace) {
        CHECK_EQUAL("PGMNRRMGOAI", p->encrypt("PRO GRAMMING"));
    }
    
    TEST_FIXTURE(Key3Fixture, StringWithNumbers) {
        // ИЗМЕНЕНО: 2025 → 2026 для соответствия таблице 5.4
        CHECK_THROW(p->encrypt("ALGORITHM2026"), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, EmptyString) {
        CHECK_THROW(p->encrypt(""), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, NoAlphaString) {
        CHECK_THROW(p->encrypt("123 !@#"), cipher_error);
    }
    
    // Тест с другим ключом (без фикстуры)
    TEST(TextShorterThanKey) {
        TableRouteCipher cipher(6);
        CHECK_THROW(cipher.encrypt("HELLO"), cipher_error);
    }
    
    // Тест с фикстурой Key5Fixture
    TEST_FIXTURE(Key5Fixture, TextEqualsKeyLength) {
        CHECK_EQUAL("HELLO", p->encrypt("HELLO"));
    }
    
    // Отдельный тест с ключом 3 (без фикстуры для разнообразия)
    TEST(TextLengthMultipleOfKey) {
        TableRouteCipher cipher(3);
        CHECK_EQUAL("ADGBEHCFI", cipher.encrypt("ABCDEFGHI"));
    }
}

// ========= ТЕСТЫ МЕТОДА DECRYPT =========
SUITE(DecryptTest) {
    // Тесты с фикстурой Key3Fixture
    TEST_FIXTURE(Key3Fixture, ValidText) {
        CHECK_EQUAL("PROGRAMMING", p->decrypt("PGMNRRMGOAI"));
    }
    
    TEST_FIXTURE(Key3Fixture, LowCaseString) {
        CHECK_EQUAL("programming", p->decrypt("pgmnrrmgoai"));
    }
    
    TEST_FIXTURE(Key3Fixture, StringWithWhitespace) {
        CHECK_EQUAL("PROGRAMMING", p->decrypt("P G M N R R M G O A I"));
    }
    
    TEST_FIXTURE(Key3Fixture, StringWithDigits) {
        CHECK_THROW(p->decrypt("CODE2026XYZ"), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, StringWithPunctuation) {
        CHECK_THROW(p->decrypt("HELLO.WORLD!"), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, EmptyString) {
        CHECK_THROW(p->decrypt(""), cipher_error);
    }
    
    TEST_FIXTURE(Key3Fixture, TextLengthMultipleOfKey) {
        CHECK_EQUAL("ABCDEFGHI", p->decrypt("ADGBEHCFI"));
    }
    
    // Тесты с другими ключами (без фикстуры)
    TEST(IncompleteTable) {
        TableRouteCipher cipher(6);
        CHECK_THROW(cipher.decrypt("HELLO"), cipher_error);
    }
    
    TEST(DecryptWithDifferentKey) {
        TableRouteCipher cipher(4);
        CHECK_THROW(cipher.decrypt("CODE2026XYZ"), cipher_error);
    }
}

// ========= ДОПОЛНИТЕЛЬНЫЕ ТЕСТЫ =========
SUITE(AdditionalTests) {
    TEST(KeyEqualsOne_EncryptDecrypt) {
        TableRouteCipher cipher(1);
        string encrypted = cipher.encrypt("HELLO");
        string decrypted = cipher.decrypt(encrypted);
        CHECK_EQUAL("HELLO", encrypted);
        CHECK_EQUAL("HELLO", decrypted);
    }
    
    TEST(EncryptDecryptCycle) {
        TableRouteCipher cipher(3);
        string original = "PROGRAMMING";
        string encrypted = cipher.encrypt(original);
        string decrypted = cipher.decrypt(encrypted);
        CHECK_EQUAL(original, decrypted);
    }
    
    TEST(MaxKey) {
        TableRouteCipher cipher(8);
        string original = "ABCDEFGH";
        string encrypted = cipher.encrypt(original);
        string decrypted = cipher.decrypt(encrypted);
        CHECK_EQUAL(original, decrypted);
    }
}

// ========= ГЛАВНАЯ ФУНКЦИЯ =========
int main() {
    cout << "Запуск модульных тестов TableRouteCipher..." << endl;
    cout << "============================================" << endl;
    
    int result = UnitTest::RunAllTests();
    
    cout << "============================================" << endl;
    cout << "Тестирование завершено." << endl;
    
    return result;
}
