#include <cstdio>
#include "textloader.h"
#include "../../framework/image.h"
#include "../../framework/textmanager.h"
#include "../../framework/engine.h"
#include "../../framework/pathmanager.h"

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
    return (str[0] == '\n' || str[0] == '\0' || str[0] == '/');
}

// Returns true if the string matches the criteria of being a title
bool is_title(char* str) {
    return (str[0] == '#');
}

// Returns the id of the title. See implementation of a list of IDs.
// Returns 0 if it's an unknown title.
int title_type(char* str) {
    if(strcmp(str, "#WORDS\n") == 0)
        return 1;
    if(strcmp(str, "#FILES\n") == 0)
        return 2;
    if(strcmp(str, "#FONTS\n") == 0)
        return 3;
    return 0;
}

static string key_def = "[]{}:";
TextLoader::Word::Word(char* str) {
    char buffer[STRING_LENGTH];
    strcpy(buffer, str);

    // Name
    char *start = strchr(buffer, key_def[0]);
    char *end =   strchr(buffer, key_def[1]);
    end[0] = '\0';
    name_ = std::string(start + 1);

    // Font
    start = strchr(end + 1, key_def[2]);
    end =   strchr(end + 1, key_def[3]);
    end[0] = '\0';
    font_ = std::string(start + 1);

    // Value
    start = strchr(end + 1, key_def[4]);
    end =   strchr(end + 1, '\n');
    if(end != NULL) end[0] = '\0';

    text_ = std::string(start + 1);
}

// Returns true if the string matches the criteria of being a key
bool TextLoader::Word::IsWord(char* str) {
    if (str[0] != '[') return false;
    char *tmp = str;
    for(int i = 1; key_def[i] != '\0'; ++i) {
        tmp = strchr(tmp, key_def[i]);
        if(tmp == NULL) return false;
    }
    return true;
}

// Fills the map with the information on the given file
bool TextLoader::Initialize(string language_file) {
    FILE* file = fopen(PATH_MANAGER()->ResolvePath(language_file).c_str(), "r");
    if(file == NULL)
        return false;

    char buffer[STRING_LENGTH];
    int reading_type = 0;

    while(!feof(file)) {
        fgets(buffer, STRING_LENGTH, file);
        if(is_blank(buffer))
            continue;

        if(is_title(buffer)) {
            reading_type = title_type(buffer);
        }

        if(reading_type == 3) {
            if(TextLoader::Font::IsFont(buffer))
                LoadFont(buffer);

        } else if(TextLoader::Word::IsWord(buffer)) {
            TextLoader::Word word = Word(buffer);
            TextLoader::Font* font = fonts_[word.font()];

            Image* val;
            if(reading_type == 2)
                val = font->LoadFile(word.text());
            else
                val = font->LoadText(word.text());

            string name = word.name();

            if(text_images_.count(name) && text_images_[name] != NULL) {
                text_images_[name]->Destroy();
                delete text_images_[name];
            }
            text_images_[name] = val;

        } else {
            // Syntax error!
        }
    }
    fclose(file);
    return true;
}

Image* TextLoader::GetImage(string text) {
    return text_images_[text];
}

void TextLoader::SetFont(std::string font) {
    if(fonts_.count(font))
        fonts_[font]->SetFont();
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


//===================================================================
//  FONT
static string font_def = "[]{}:";
void TextLoader::LoadFont(char* str) {
    char buffer[STRING_LENGTH];
    strcpy(buffer, str);

    char *start = strchr(buffer, font_def[0]);
    char *end =   strchr(buffer, font_def[1]);
    end[0] = '\0';
    string name = std::string(start + 1);
    end[0] = font_def[1];

    int font_size = 50;
    char ident = 'c';
    bool style = false;

    char* arg1 = strchr(buffer, '{');
    char* arg2 = strchr(arg1, '}');
    arg2[0] = '\0';
    font_size = atoi(arg1 + 1);
    arg2[0] = '}';

    if(arg2[1] != ':') {
        if(arg2[1] == '+')
            style = true;
        else {
            ident = arg2[1];
            style = (arg2[2] == '+');
        }
    }
    char *resp = strchr(buffer, ':');

    end =   strchr(buffer + 1, '\n');
    if(end != NULL) end[0] = '\0';

    fonts_[name] = new Font(resp + 1, font_size, ident, style);
}

TextLoader::Font::Font(std::string filepath, int size, char indent, bool style) {
    filepath_ = filepath;
    size_ = size;
    indent_ = indent;
    style_ = style;
}
Image* TextLoader::Font::LoadText(string str) {
    SetFont();
    if(style_)
        return TEXT_MANAGER()->LoadFancyLine(str.c_str());
    else
        return TEXT_MANAGER()->LoadLine(str.c_str());
}
Image* TextLoader::Font::LoadFile(string filepath) {
    SetFont();
    return TEXT_MANAGER()->LoadFile(filepath.c_str(), indent_);
}
void TextLoader::Font::SetFont() {
    TEXT_MANAGER()->setFont(filepath_, size_, NULL);
}
bool TextLoader::Font::IsFont(char *str) {
    if (str[0] != '[') return false;
    char *tmp = str;
    for(int i = 1; font_def[i] != '\0'; ++i) {
        tmp = strchr(tmp, font_def[i]);
        if(tmp == NULL) return false;
    }
    return true;
}

}
