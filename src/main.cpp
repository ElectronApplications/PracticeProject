#include <iostream>
#include <string>
#include <map>
using namespace std;

const wchar_t alphabet[] = {' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                        ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
                        'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', L'А', L'Б', L'В', L'Г', L'Д', L'Е', L'Ж', L'З',
                        L'И', L'Й', L'К', L'Л', L'М', L'Н', L'О', L'П', L'Р', L'С', L'Т', L'У', L'Ф', L'Х', L'Ц', L'Ч', L'Ш', L'Щ', L'Ъ', L'Ы', L'Ь', L'Э',
                        L'Ю', L'Я', L'а', L'б', L'в', L'г', L'д', L'е', L'ж', L'з', L'и', L'й', L'к', L'л', L'м', L'н', L'о', L'п', L'р', L'с', L'т', L'у',
                        L'ф', L'х', L'ц', L'ч', L'ш', L'щ', L'ъ', L'ы', L'ь', L'э', L'ю', L'я'};
map<wchar_t, int> reverse_alphabet;
const int n = sizeof(alphabet) / sizeof(wchar_t);

int symbol_encrypt(int symbol, int key) {
    return ((symbol + key) % n + n) % n;
}

int symbol_decrypt(int symbol, int key) {
    return ((symbol - key) % n + n) % n;
}

wstring message_encrypt(wstring message, wstring key) {
    wstring result = L"";
    for(int i = 0; i < message.length(); i++) {
        int symbol = reverse_alphabet[message[i]];
        int key_symbol = reverse_alphabet[key[i % key.length()]];
        result += alphabet[symbol_encrypt(symbol, key_symbol)];
    }
    return result;
}

wstring message_decrypt(wstring message, wstring key) {
    wstring result = L"";
    for(int i = 0; i < message.length(); i++) {
        int symbol = reverse_alphabet[message[i]];
        int key_symbol = reverse_alphabet[key[i % key.length()]];
        result += alphabet[symbol_decrypt(symbol, key_symbol)];
    }
    return result;
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    for(int i = 0; i < n; i++) {
        reverse_alphabet[alphabet[i]] = i;
    }

    wstring message, key;

    getline(wcin, message);
    getline(wcin, key);

    wcout << message_encrypt(message, key) << endl;
    wcout << message_decrypt(message_encrypt(message, key), key) << endl;

    return 0;
}