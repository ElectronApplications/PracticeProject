#include <string>
#include <map>
#include <vector>
#include <codecvt>
#include <locale>

using namespace std;

struct Dictionary {
    std::vector<char32_t> dict;
    std::map<char32_t, int> reverse_dict;
    int n;

    Dictionary(std::vector<char32_t> dict);
};

extern Dictionary full_dictionary;
extern Dictionary ru_dictionary;
extern Dictionary en_dictionary;

std::string message_encrypt(std::string message, std::string key, Dictionary &dict);
std::string message_decrypt(std::string message, std::string key, Dictionary &dict);
