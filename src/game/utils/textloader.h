#ifndef HORUSEYE_GAME_UTILS_TEXTLOADER_H_
#define HORUSEYE_GAME_UTILS_TEXTLOADER_H_

#include <string>
#include <map>
#include <ugdk/graphic.h>

namespace utils {

using std::wstring;

#define TEXT_LOADER() (utils::TextLoader::reference())

class TextLoader {
  public:
    static TextLoader* reference() {
        static TextLoader* r = new TextLoader();
        return r;
    }
    ~TextLoader();

    bool Initialize(std::string language_file);
    ugdk::Drawable* GetImage(const std::string& text);
	ugdk::Drawable* GetImage(const std::wstring& text);
    //void SetFont(std::string font);

    bool Clear();

  private:
    TextLoader();

    class Font {
      public:
        static bool IsFont(wchar_t *);
    };
    class Word {
      public:
        Word(wchar_t* str, bool from_file);
        ~Word() {}

        const std::wstring& name() const { return name_; }
        const std::wstring& font() const { return font_; }
		const std::wstring& text() const { return text_; }
        ugdk::Text* ConvertToText() const;

        static bool IsWord(wchar_t *);

      private:
        std::wstring name_, font_, text_;
        bool from_file_;
    };
    void ReadFont(wchar_t *str);

    std::map<std::wstring, Word*> text_images_;
};

}

#endif /* HORUSEYE_GAME_UTILS_TEXTLOADER_H_ */
