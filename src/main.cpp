#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>

#include "vigenere.h"

#ifdef _WIN32
#include "windows.h"
#endif

using namespace std;

// Функция для чтения строки из консоли в кодировке UTF-8
string console_input() {
    string input;

    #ifndef _WIN32
        getline(cin, input); // Под *nix системами сразу читаем строку в кодировке utf-8
        return input;
    #else
        SetConsoleOutputCP(866);
        getline(cin, input); // Под windows читаем строку в кодировке cp866
        SetConsoleOutputCP(65001);
        string result;
        for(char i : input) { // Каждый символ русского алфавита в строке переводим в utf-8
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

void interactive() {
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
}

void crypt_file(string input_path, string output_path, string key, bool encrypt, Dictionary &dict) {
    ifstream input_file(input_path);
    string input((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
    
    string output;
    try {
        if(encrypt)
            output = message_encrypt(input, key, dict);
        else
            output = message_decrypt(input, key, dict);
    }
    catch(const invalid_argument &e) {
        cerr << e.what() << endl;
        exit(-1);
    }

    ofstream output_file(output_path, ios::trunc);
    output_file << output;

    if(encrypt)
        cout << "Содержимое файла " << input_path << " было успешно зашифровано и записано в файл " << output_path << endl;
    else
        cout << "Содержимое файла " << input_path << " было успешно расшифровано и записано в файл " << output_path << endl;
}

void help(char *program_name) {
    cout << "Использование: " << program_name << " [-i] [-e,d] input key [output] [dict]" << endl;
    cout << "       Флаг -i запускает программу в интерактивном режиме" << endl;
    cout << "       Флаг -e указывает программе, что содержимое файла надо шифровать. Этот флаг является флагом по умолчанию" << endl;
    cout << "       Флаг -d указывает программе, что содержимое файла надо расшифровать" << endl;
    cout << "       input - название файла, содержимое которого надо зашифровать или расшифровать" << endl;
    cout << "       output - название файла, в который надо записывать результат. По умолчанию используется название output.txt" << endl;
    cout << "       key - ключ, используемый для шифровки и расшифровки" << endl;
    cout << "       dict - используемый словарь. Принимает значения full, ru, en" << endl;
}

int main(int argc, char **argv) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    #ifdef _WIN32
        SetConsoleOutputCP(65001); // Используем кодировку utf-8 для вывода в консоль
    #endif

    if(argc >= 2 && string(argv[1]) == "-i")
        interactive();
    else if(argc < 3) {
        help(argv[0]);
    }
    else if(argc >= 3) {
        int index = 1;
        bool encrypt = true;
        if(string(argv[1]) == "-e")
            index++;
        else if(string(argv[1]) == "-d") {
            encrypt = false;
            index++;
        }

        if(argc >= 2 + index) {
            string input_path = argv[index];
            string key = argv[index + 1];
            string output_path = "output.txt";
            Dictionary &dict = full_dictionary;

            if(argc >= 3 + index)
                output_path = argv[index + 2];
            
            if(argc >= 4 + index) {
                if(string(argv[3 + index]) == "ru")
                    dict = ru_dictionary;
                else if(string(argv[3 + index]) == "en")
                    dict = en_dictionary;
            }
            
            crypt_file(input_path, output_path, key, encrypt, dict);
        }
        else
            help(argv[0]);
    }
    
    return 0;
}
