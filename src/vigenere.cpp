#include <string>
#include <map>
#include <vector>
#include <codecvt>
#include <locale>
#include <stdexcept>

#include "vigenere.h"

const vector<char32_t> full_dict = {'!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                        ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
                        'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', U'А', U'Б', U'В', U'Г', U'Д', U'Е', U'Ё', U'Ж', U'З',
                        U'И', U'Й', U'К', U'Л', U'М', U'Н', U'О', U'П', U'Р', U'С', U'Т', U'У', U'Ф', U'Х', U'Ц', U'Ч', U'Ш', U'Щ', U'Ъ', U'Ы', U'Ь', U'Э',
                        U'Ю', U'Я', U'а', U'б', U'в', U'г', U'д', U'е', U'ё', U'ж', U'з', U'и', U'й', U'к', U'л', U'м', U'н', U'о', U'п', U'р', U'с', U'т', U'у',
                        U'ф', U'х', U'ц', U'ч', U'ш', U'щ', U'ъ', U'ы', U'ь', U'э', U'ю', U'я'};
const vector<char32_t> ru_dict = {U'А', U'Б', U'В', U'Г', U'Д', U'Е', U'Ё', U'Ж', U'З',
                        U'И', U'Й', U'К', U'Л', U'М', U'Н', U'О', U'П', U'Р', U'С', U'Т', U'У', U'Ф', U'Х', U'Ц', U'Ч', U'Ш', U'Щ', U'Ъ', U'Ы', U'Ь', U'Э',
                        U'Ю', U'Я', U'а', U'б', U'в', U'г', U'д', U'е', U'ё', U'ж', U'з', U'и', U'й', U'к', U'л', U'м', U'н', U'о', U'п', U'р', U'с', U'т', U'у',
                        U'ф', U'х', U'ц', U'ч', U'ш', U'щ', U'ъ', U'ы', U'ь', U'э', U'ю', U'я'};
const vector<char32_t> en_dict = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
                        'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

Dictionary full_dictionary(full_dict, 1);
Dictionary ru_dictionary(ru_dict, 2);
Dictionary en_dictionary(en_dict, 2);

Dictionary::Dictionary(vector<char32_t> dict, int repeat) {
    for(int i = 0; i < dict.size(); i++) {
        this->reverse_dict[dict[i]] = i % (dict.size() / repeat);
    }

    for(int i = 0; i < dict.size() / repeat; i++) {
        this->dict.push_back(dict[i]);
    }
    this->n = this->dict.size();
}

// Функция шифровки символа symbol ключом key при использовании алфавита с количеством символов n
int symbol_encrypt(int symbol, int key, int n) {
    return ((symbol + key) % n + n) % n;
}

// Функция расшифровки символа symbol ключом key при использовании алфавита с количеством символов n
int symbol_decrypt(int symbol, int key, int n) {
    return ((symbol - key) % n + n) % n;
}

// Функция для проверки строки
void validate_message(u32string message, Dictionary &dict) {
    for(char32_t i : message) {
        if(dict.reverse_dict.find(i) == dict.reverse_dict.end() && i > ' ')
            throw invalid_argument("Ошибка! В тексте присутствуют символы, которых нет в выбранном алфавите!");
    }
}

// Функция для проверки ключа
void validate_key(u32string key, Dictionary &dict) {
    int actual_symbols = 0;
    for(char32_t i : key) {
        if(dict.reverse_dict.find(i) != dict.reverse_dict.end())
            actual_symbols++;
        else if(i > ' ')
            throw invalid_argument("Ошибка! В ключе присутствуют символы, которых нет в выбранном алфавите!");
    }

    if(actual_symbols == 0)
        throw invalid_argument("Ошибка! В ключе отсутствуют символы из алфавита!");
}

// Функция шифровки строки
string message_encrypt(string message, string key, Dictionary &dict) {
    wstring_convert<codecvt_utf8<char32_t>, char32_t> convert;

    u32string message32 = convert.from_bytes(message); // Переводим строку в UTF-32, чтобы иметь возможность считывать отдельные юникод символы
    u32string key32 = convert.from_bytes(key); // Переводим ключ в UTF-32

    validate_message(message32, dict);
    validate_key(key32, dict);

    u32string result = U"";
    int index = 0; // Номер текущего символа ключа
    for(int i = 0; i < message32.length(); i++) {
        char32_t msgc = message32[i];
        char32_t keyc = key32[index % key32.length()];

        if(dict.reverse_dict.find(msgc) == dict.reverse_dict.end() && msgc <= ' ') {
            result += msgc;
            continue; // Игнорируем спец. символы в строке
        }
        else if(dict.reverse_dict.find(keyc) == dict.reverse_dict.end() && keyc <= ' ') {
            index++;
            i--;
            continue; // Игнорируем спец. символы в ключе
        }

        int symbol = dict.reverse_dict[msgc];
        int key_symbol = dict.reverse_dict[keyc];
        result += dict.dict[symbol_encrypt(symbol, key_symbol, dict.n)];
    
        index++;
    }
    return convert.to_bytes(result);
}

// Функция расшифровки строки
string message_decrypt(string message, string key, Dictionary &dict) {
    wstring_convert<codecvt_utf8<char32_t>, char32_t> convert;

    u32string message32 = convert.from_bytes(message); // Переводим строку в UTF-32, чтобы иметь возможность считывать отдельные юникод символы
    u32string key32 = convert.from_bytes(key); // Переводим ключ в UTF-32

    validate_message(message32, dict);
    validate_key(key32, dict);

    u32string result = U"";
    int index = 0; // Номер текущего символа ключа
    for(int i = 0; i < message32.length(); i++) {
        char32_t msgc = message32[i];
        char32_t keyc = key32[index % key32.length()];

        if(dict.reverse_dict.find(msgc) == dict.reverse_dict.end() && msgc <= ' ') {
            result += msgc;
            continue; // Игнорируем спец. символы в строке
        }
        else if(dict.reverse_dict.find(keyc) == dict.reverse_dict.end() && keyc <= ' ') {
            index++;
            i--;
            continue; // Игнорируем спец. символы в ключе
        }

        int symbol = dict.reverse_dict[msgc];
        int key_symbol = dict.reverse_dict[keyc];
        result += dict.dict[symbol_decrypt(symbol, key_symbol, dict.n)];
    
        index++;
    }
    return convert.to_bytes(result);
}