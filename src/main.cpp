#include <iostream>
#include <string>
#include <stdexcept>

#include "vigenere.h"

#ifdef _WIN32
#include "windows.h"
#endif

using namespace std;

string console_input() {
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

Dictionary &choose_dict() {
    cout << "Выберите используемый алфавит:" << endl;
    cout << "1) Полный алфавит - содержит строчные и прописные буквы русского и английского алфавитов, цифры и специальные символы" << endl;
    cout << "2) Русский алфавит - содержит только прописные буквы русского алфавита" << endl;
    cout << "3) Английский алфавит - содержит только прописные буквы английского алфавита" << endl;
    
    int input;
    cin >> input;

    switch(input) {
        default: case 1:
            return full_dictionary;
        case 2:
            return ru_dictionary;
        case 3:
            return en_dictionary;
    }
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    #ifdef _WIN32
        SetConsoleOutputCP(65001);
    #endif

    int input;
    do {
        cout << "=====================" << endl;
        cout << "Введите действие:" << endl;
        cout << "1) Зашифровать строку" << endl;
        cout << "2) Расшифровать строку" << endl;
        cout << "3) Выйти из программы" << endl;
        cin >> input;
        
        switch(input) {
            case 1: {
                Dictionary &dict = choose_dict();

                string message, key;
                cin.ignore();
                cout << "Введите строку, которую нужно зашифровать: " << endl;
                message = console_input();
                cout << "Введите ключ, при помощи которого вы хотите зашифровать введенную строку: " << endl;
                key = console_input();

                try {
                    cout << "Зашифрованная строка: \"" << message_encrypt(message, key, dict) << "\"" << endl;
                }
                catch(const invalid_argument &e) {
                    cout << e.what() << endl;
                }
                break;
            }
            case 2: {
                Dictionary &dict = choose_dict();

                string message, key;
                cin.ignore();
                cout << "Введите строку, которую нужно расшифровать: " << endl;
                message = console_input();
                cout << "Введите ключ, при помощи которого данная строка была зашифрована: " << endl;
                key = console_input();

                try {
                    cout << "Расшифрованная строка: \"" << message_decrypt(message, key, dict) << "\"" << endl;
                }
                catch(const invalid_argument &e) {
                    cout << e.what() << endl;
                }
                break;
            }
        }
    } while(input != 3);
    
    return 0;
}
