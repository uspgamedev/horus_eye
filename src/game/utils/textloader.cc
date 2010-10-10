#include <cstdio>
#include "textloader.h"
#include "../../framework/image.h"
#include "../../framework/textmanager.h"
#include "../../framework/engine.h"

namespace utils {

using namespace framework;
using std::map;
using std::string;

#define STRING_LENGTH 1024

TextLoader::TextLoader() {
    text_images_.clear();
}

TextLoader::~TextLoader() {
    Clear();
}

// Returns true if the first character in the string is a newline or \0
bool is_blank(char* str) {
    return (str[0] == '\n' || str[0] == '\0');
}

// Returns true if the string matches the criteria of being a title
bool is_title(char* str) {
    return (str[0] == '#');
}

// Returns true if the string matches the criteria of being a key
bool is_key(char* str) {
    if (str[0] != '[') return false;
    for(int i = 1; i < STRING_LENGTH; ++i) {
        if(str[i] == '\0') return false;
        if(str[i-1] == ']') {
            return (str[i] == ':');
        }
    }
    return false;
}

// Returns the id of the title. See implementation of a list of IDs.
// Returns 0 if it's an unknown title.
int title_type(char* str) {
    if(strcmp(str, "#WORDS\n") == 0)
        return 1;
    if(strcmp(str, "#FILES\n") == 0)
        return 2;
    return 0;
}

// Returns the name of a key. Does not check if given string is actually a key
string key_name(char* str) {
    char buffer[STRING_LENGTH];
    for(int i = 1; (i < STRING_LENGTH) && (str[i] != '\0'); ++i) {
        if(str[i] == ']') {
            buffer[i-1] = '\0';
            break;
        } else
            buffer[i-1] = str[i];
    }
    return std::string(buffer);
}

// Returns the value of a key. Does not check if given string is actually a key
string key_val(char* str) {
    char buffer[STRING_LENGTH];
    bool start_copy = false;
    int copy = 0;
    for(int i = 0; i < STRING_LENGTH; ++i) {
        if(start_copy) {
            if(str[i] == '\n' || str[i] == '\0') {
                buffer[copy] = '\0';
                break;
            }
            buffer[copy++] = str[i];
        } else if(str[i] == ':')
            start_copy = true;
    }
    return std::string(buffer);
}

// Fills the map with the information on the given file
bool TextLoader::Initialize(string language_file) {
    FILE* file = fopen(language_file.c_str(), "r");
    if(file == NULL)
        return false;
    char buffer[STRING_LENGTH];
    bool load_from_file = false;
    TEXT_MANAGER()->setFont("data/font/Filmcrypob.ttf", 50, NULL);
    while(!feof(file)) {
        fgets(buffer, STRING_LENGTH, file);
        if(is_blank(buffer))
            continue;
        if(is_title(buffer))
            load_from_file = (title_type(buffer) == 2);
        else if(is_key(buffer)) {
            string name = key_name(buffer);
            Image* val;
            if(load_from_file)
                val = TEXT_MANAGER()->LoadFile(key_val(buffer).c_str(), 'c');
            else
                val = TEXT_MANAGER()->LoadFancyLine(key_val(buffer).c_str());

            if(text_images_.count(name.c_str()) && text_images_[name.c_str()] != NULL) {
                text_images_[name.c_str()]->Destroy();
                delete text_images_[name.c_str()];
            }
            text_images_[name.c_str()] = val;

        } else {
            // Syntax error!
        }
    }
    fclose(file);
    return true;
}

Image* TextLoader::GetImage(string text) {
    return text_images_[text.c_str()];
}

bool TextLoader::Clear() {
    map<string, Image*>::iterator it;
    for(it = text_images_.begin(); it != text_images_.end(); ++it) {
        it->second->Destroy();
        delete it->second;
    }
    text_images_.clear();
    return true;
}

}

