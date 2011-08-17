#include <cstdio>
#include "textloader.h"
#include "../../framework/image.h"
#include "../../framework/textmanager.h"
#include "../../framework/engine.h"
#include "../../framework/pathmanager.h"
#include "../../framework/text.h"

namespace utils {

using namespace ugdk;
using std::map;
using std::string;

#define STRING_LENGTH 1024

char convertToChar(wchar_t t) {
	char c = static_cast<char>(static_cast<int>(t));
	return c;
}

TextLoader::TextLoader() {
    text_images_.clear();
}

TextLoader::~TextLoader() {
    Clear();
}

// Returns true if the first character in the string is a newline or \0
bool is_blank(wchar_t* str) {
    return (str[0] == '\n' || str[0] == '\0' || str[0] == '/');
}

// Returns true if the string matches the criteria of being a title
bool is_title(wchar_t* str) {
    return (str[0] == '#');
}

// Returns the id of the title. See implementation of a list of IDs.
// Returns 0 if it's an unknown title.
int title_type(wchar_t* str) {
    if(wcscmp(str, L"#WORDS\n") == 0)
        return 1;
    if(wcscmp(str, L"#FILES\n") == 0)
        return 2;
    if(wcscmp(str, L"#FONTS\n") == 0)
        return 3;
    return 0;
}

static wstring key_def = L"[]{}:\0";
TextLoader::Word::Word(wchar_t* str) {
    wchar_t buffer[STRING_LENGTH];
    wcscpy(buffer, str);

    // Name
    wchar_t *start = wcschr(buffer, key_def[0]);
    wchar_t *end =   wcschr(buffer, key_def[1]);
    end[0] = L'\0';
    name_ = std::wstring(start + 1);

    // Font
    start = wcschr(end + 1, key_def[2]);
    end =   wcschr(end + 1, key_def[3]);
    end[0] = L'\0';
    font_ = std::wstring(start + 1);

    // Value
    start = wcschr(end + 1, key_def[4]);
    end =   wcschr(end + 1, L'\n');
    if(end != NULL) end[0] = L'\0';

    text_ = std::wstring(start + 1);
}

// Returns true if the string matches the criteria of being a key
bool TextLoader::Word::IsWord(wchar_t* str) {
    if (str[0] != L'[') return false;
    wchar_t *tmp = str;
    for(int i = 1; i<5/*key_def[i] != '\0'*/; ++i) {
        tmp = wcschr(tmp, key_def[i]);
        if(tmp == NULL) return false;
    }
    return true;
}

// Fills the map with the information on the given file
bool TextLoader::Initialize(string language_file) {
    FILE* file = fopen(PATH_MANAGER()->ResolvePath(language_file).c_str(), "r,ccs=UTF-8");
    if(file == NULL)
        return false;

    Clear();

    wchar_t buffer[STRING_LENGTH];
    int reading_type = 0;

    while(!feof(file)) {
        fgetws(buffer, STRING_LENGTH, file);
        if(is_blank(buffer))
            continue;

        if(is_title(buffer)) {
            reading_type = title_type(buffer);
        }

        if(reading_type == 3) {
            if(TextLoader::Font::IsFont(buffer))
                ReadFont(buffer);

        } else if(TextLoader::Word::IsWord(buffer)) {
            TextLoader::Word word = Word(buffer);
            //TextLoader::Font* font = fonts_[word.font()];

            Drawable* val;
            if(reading_type == 2) // Reading tags of {FROM_FILE} type.
				val = TEXT_MANAGER()->GetTextFromFile(word.text(), word.font());
            else
				val = TEXT_MANAGER()->GetText(word.text(), word.font());

			if(val == NULL) // Don't store NULL drawables. 
				continue;

            wstring name = word.name();

			// Defensive Programming! Erase previous drawable with same name 
			// before overwriting.
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

Drawable* TextLoader::GetImage(string text) {
	std::wstring final(text.length(), L' ');
	std::copy(text.begin(), text.end(), final.begin());
	return text_images_[final];
}
Drawable* TextLoader::GetImage(wstring text) {
	return text_images_[text];
}

/*void TextLoader::SetFont(std::string font) {
    if(fonts_.count(font))
        fonts_[font]->SetFont();
}*/

bool TextLoader::Clear() {
    map<wstring, Drawable*>::iterator it;
    for(it = text_images_.begin(); it != text_images_.end(); ++it) {
		if(it->second != NULL) {
			it->second->Destroy();
			delete it->second;
		}
    }
    text_images_.clear();
    return true;
}


//===================================================================
//  FONT
static wstring font_def = L"[]{}:\0";
void TextLoader::ReadFont(wchar_t* str) {
    wchar_t buffer[STRING_LENGTH];
    wcscpy(buffer, str);

    wchar_t *start = wcschr(buffer, font_def[0]);
    wchar_t *end =   wcschr(buffer, font_def[1]);
    end[0] = L'\0';
    wstring name = std::wstring(start + 1);
    end[0] = font_def[1];

    int font_size = 50;
    char ident = 'c';
    bool style = false;

    wchar_t* arg1 = wcschr(buffer, L'{');
    wchar_t* arg2 = wcschr(arg1, L'}');
    arg2[0] = L'\0';
	swscanf(arg1 + 1, L"%d", &font_size);
    arg2[0] = L'}';

    if(arg2[1] != L':') {
        if(arg2[1] == L'+')
            style = true;
        else {
            ident = convertToChar(arg2[1]);
            style = (arg2[2] == L'+');
        }
    }
    wchar_t *resp = wcschr(buffer, L':');

    end =   wcschr(buffer + 1, L'\n');
    if(end != NULL) end[0] = L'\0';

	TEXT_MANAGER()->AddFont(name, resp + 1, font_size, ident, style);
}

bool TextLoader::Font::IsFont(wchar_t *str) {
    if (str[0] != L'[') return false;
    wchar_t *tmp = str;
    for(int i = 1; i<5/*font_def[i] != '\0'*/; ++i) {
        tmp = wcschr(tmp, font_def[i]);
        if(tmp == NULL) return false;
    }
    return true;
}

}
