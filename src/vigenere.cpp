#include <string>
#include <map>
#include <vector>
#include <codecvt>
#include <locale>
#include <stdexcept>

#include "vigenere.h"

const vector<char32_t> full_dict = {' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                        ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
                        'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', U'А', U'Б', U'В', U'Г', U'Д', U'Е', U'Ё', U'Ж', U'З',
                        U'И', U'Й', U'К', U'Л', U'М', U'Н', U'О', U'П', U'Р', U'С', U'Т', U'У', U'Ф', U'Х', U'Ц', U'Ч', U'Ш', U'Щ', U'Ъ', U'Ы', U'Ь', U'Э',
                        U'Ю', U'Я', U'а', U'б', U'в', U'г', U'д', U'е', U'ё', U'ж', U'з', U'и', U'й', U'к', U'л', U'м', U'н', U'о', U'п', U'р', U'с', U'т', U'у',
                        U'ф', U'х', U'ц', U'ч', U'ш', U'щ', U'ъ', U'ы', U'ь', U'э', U'ю', U'я'};
const vector<char32_t> ru_dict = {U'а', U'б', U'в', U'г', U'д', U'е', U'ё', U'ж', U'з', U'и', U'й', U'к', U'л', U'м', U'н', U'о', U'п', U'р', U'с', U'т', U'у',
                        U'ф', U'х', U'ц', U'ч', U'ш', U'щ', U'ъ', U'ы', U'ь', U'э', U'ю', U'я'};
const vector<char32_t> en_dict = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

Dictionary full_dictionary(full_dict);
Dictionary ru_dictionary(ru_dict);
Dictionary en_dictionary(en_dict);

Dictionary::Dictionary(vector<char32_t> dict) {
    this->dict = dict;
    this->n = this->dict.size();

    for(int i = 0; i < this->n; i++) {
        this->reverse_dict[this->dict[i]] = i;
    }
}

int symbol_encrypt(int symbol, int key, int n) {
    return ((symbol + key) % n + n) % n;
}

int symbol_decrypt(int symbol, int key, int n) {
    return ((symbol - key) % n + n) % n;
}

string message_encrypt(string message, string key, Dictionary &dict) {
    wstring_convert<codecvt_utf8<char32_t>, char32_t> convert;

    u32string message32 = convert.from_bytes(message);
    u32string key32 = convert.from_bytes(key);
    u32string result = U"";

    for(int i = 0; i < message32.length(); i++) {
        if(dict.reverse_dict.find(message32[i]) == dict.reverse_dict.end() || dict.reverse_dict.find(key32[i % key32.length()]) == dict.reverse_dict.end())
            throw invalid_argument("Ошибка! В строке присутствуют символы, которых нет в выбранном алфавите!");

        int symbol = dict.reverse_dict[message32[i]];
        int key_symbol = dict.reverse_dict[key32[i % key32.length()]];
        result += dict.dict[symbol_encrypt(symbol, key_symbol, dict.n)];
    }
    return convert.to_bytes(result);
}

string message_decrypt(string message, string key, Dictionary &dict) {
    wstring_convert<codecvt_utf8<char32_t>, char32_t> convert;

    u32string message32 = convert.from_bytes(message);
    u32string key32 = convert.from_bytes(key);
    u32string result = U"";
    for(int i = 0; i < message32.length(); i++) {
        if(dict.reverse_dict.find(message32[i]) == dict.reverse_dict.end() || dict.reverse_dict.find(key32[i % key32.length()]) == dict.reverse_dict.end())
            throw invalid_argument("Ошибка! В строке присутствуют символы, которых нет в выбранном алфавите!");

        int symbol = dict.reverse_dict[message32[i]];
        int key_symbol = dict.reverse_dict[key32[i % key32.length()]];
        result += dict.dict[symbol_decrypt(symbol, key_symbol, dict.n)];
    }
    return convert.to_bytes(result);
}