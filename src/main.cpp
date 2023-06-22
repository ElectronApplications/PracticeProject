#include <iostream>
#include <string>
#include <map>
#include <codecvt>
#include <locale>

#ifdef _WIN32
#include "windows.h"
#endif

using namespace std;

const char32_t dictionary[] = {' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                        ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
                        'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', U'А', U'Б', U'В', U'Г', U'Д', U'Е', U'Ё', U'Ж', U'З',
                        U'И', U'Й', U'К', U'Л', U'М', U'Н', U'О', U'П', U'Р', U'С', U'Т', U'У', U'Ф', U'Х', U'Ц', U'Ч', U'Ш', U'Щ', U'Ъ', U'Ы', U'Ь', U'Э',
                        U'Ю', U'Я', U'а', U'б', U'в', U'г', U'д', U'е', U'ё', U'ж', U'з', U'и', U'й', U'к', U'л', U'м', U'н', U'о', U'п', U'р', U'с', U'т', U'у',
                        U'ф', U'х', U'ц', U'ч', U'ш', U'щ', U'ъ', U'ы', U'ь', U'э', U'ю', U'я'};
map<char32_t, int> reverse_dictionary;
const int n = sizeof(dictionary) / sizeof(char32_t);

int symbol_encrypt(int symbol, int key) {
    return ((symbol + key) % n + n) % n;
}

int symbol_decrypt(int symbol, int key) {
    return ((symbol - key) % n + n) % n;
}

string message_encrypt(string message, string key) {
    wstring_convert<codecvt_utf8<char32_t>, char32_t> convert;

    u32string message32 = convert.from_bytes(message);
    u32string key32 = convert.from_bytes(key);
    u32string result = U"";

    for(int i = 0; i < message32.length(); i++) {
        int symbol = reverse_dictionary[message32[i]];
        int key_symbol = reverse_dictionary[key32[i % key32.length()]];
        result += dictionary[symbol_encrypt(symbol, key_symbol)];
    }
    return convert.to_bytes(result);
}

string message_decrypt(string message, string key) {
    wstring_convert<codecvt_utf8<char32_t>, char32_t> convert;

    u32string message32 = convert.from_bytes(message);
    u32string key32 = convert.from_bytes(key);
    u32string result = U"";
    for(int i = 0; i < message32.length(); i++) {
        int symbol = reverse_dictionary[message32[i]];
        int key_symbol = reverse_dictionary[key32[i % key32.length()]];
        result += dictionary[symbol_decrypt(symbol, key_symbol)];
    }
    return convert.to_bytes(result);
}

string console_input() {
    cin.ignore();
    string input;

    #ifndef _WIN32
        getline(cin, input);
        return input;
    #else
        SetConsoleOutputCP(866);
        getline(cin, input);
        SetConsoleOutputCP(65001);
        string result;
        for(char i : input) {
            if(i >= '\x80' && i <= '\xAF') {
                result += '\xd0';
                result += ('\x90' + (i - '\x80'));
            }
            else if(i >= '\xE0' && i <= '\xEF') {
                result += '\xd1';
                result += ('\x80' + (i - '\xE0'));
            }
            else if(i == '\xF0')
                result += "Ё";
            else if(i == '\xF1')
                result += "ё";
            else
                result += i;
        }
        return result;
    #endif
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    #ifdef _WIN32
        SetConsoleOutputCP(65001);
    #endif

    for(int i = 0; i < n; i++) {
        reverse_dictionary[dictionary[i]] = i;
    }

    int input = 1;
    do {
        cout << "Введите действие:" << endl;
        cout << "1) Зашифровать строку" << endl;
        cout << "2) Расшифровать строку" << endl;
        cout << "3) Выйти из программы" << endl;
        cin >> input;
        
        switch(input) {
            case 1: {
                string message, key;
                cout << "Введите строку, которую нужно зашифровать: " << endl;
                message = console_input();
                cout << "Введите ключ, при помощи которого вы хотите зашифровать введенную строку: " << endl;
                key = console_input();
        
                cout << "Зашифрованная строка: \"" << message_encrypt(message, key) << "\"" << endl;
                break;
            }
            case 2: {
                string message, key;
                cout << "Введите строку, которую нужно расшифровать: " << endl;
                message = console_input();
                cout << "Введите ключ, при помощи которого данная строка была зашифрована: " << endl;
                key = console_input();

                cout << "Расшифрованная строка: \"" << message_decrypt(message, key) << "\"" << endl;
                break;
            }
        }
    } while(input != 3);
    
    return 0;
}
