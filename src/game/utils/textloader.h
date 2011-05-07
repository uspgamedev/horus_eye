#ifndef HORUSEYE_GAME_UTILS_TEXTLOADER_H_
#define HORUSEYE_GAME_UTILS_TEXTLOADER_H_

#include <string>
#include <map>

namespace framework {
class Text;
class Drawable;
class Image;
}

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
    framework::Drawable* GetImage(std::string text);
	framework::Drawable* GetImage(std::wstring text);
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
        Word(wchar_t* str);
        ~Word() {}

        std::wstring& name() { return name_; }
        std::wstring& font() { return font_; }
		std::wstring& text() { return text_; }

        static bool IsWord(wchar_t *);

      private:
        std::wstring name_, font_, text_;
    };
    void ReadFont(wchar_t *str);

    std::map<std::wstring, framework::Drawable*> text_images_;
};

}

#endif /* HORUSEYE_GAME_UTILS_TEXTLOADER_H_ */
