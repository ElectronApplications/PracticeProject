#include <string>
#include <map>
#include <vector>
#include <codecvt>
#include <locale>

using namespace std;

struct Dictionary {
    std::vector<char32_t> dict; // Таблица перевода номера в символ
    std::map<char32_t, int> reverse_dict; // Таблица перевода символа в его номер
    int n; // Количество символов в словаре

    /**
     * @param dict Массив всех символов. Может повторяться, чтобы иметь возможность приводить разные символы к одному номеру
     * @param repeat Количество повторений массива
    */
    Dictionary(std::vector<char32_t> dict, int repeat);
};

extern Dictionary full_dictionary;
extern Dictionary ru_dictionary;
extern Dictionary en_dictionary;

/**
 * @brief Функция шифровки строки с использованием шифра Виженере
 * @param message Строка в кодировке utf-8, которую нужно зашифровать
 * @param key Ключ в кодировке utf-8, используемый для шифрования
 * @param dict Словарь символов, используемых для шифрования
*/
std::string message_encrypt(std::string message, std::string key, Dictionary &dict);

/**
 * @brief Функция расшифровки строки с использованием шифра Виженере
 * @param message Строка в кодировке utf-8, которую нужно расшифровать
 * @param key Ключ в кодировке utf-8, который был использован при шифровке
 * @param dict Словарь символов, который был использован при шифровке
*/
std::string message_decrypt(std::string message, std::string key, Dictionary &dict);
