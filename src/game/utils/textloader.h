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
    //void SetFont(std::string font);

    bool Clear();

  private:
    TextLoader();

    class Font {
      public:
        Font(std::string filepath, int size, char indent, bool style);
        ~Font() {}

        static bool IsFont(char *);

      private:
        std::string filepath_;
        int size_;
        char indent_;
        bool style_;
    };
    class Word {
      public:
        Word(char* str);
        ~Word() {}

        std::string& name() { return name_; }
        std::string& font() { return font_; }
        std::string& text() { return text_; }

        static bool IsWord(char *);

      private:
        std::string name_, font_, text_;
    };
    void ReadFont(char *str);

    std::map<std::string, framework::Drawable*> text_images_;
    std::map<std::string, TextLoader::Font*> fonts_;

};

}

#endif /* HORUSEYE_GAME_UTILS_TEXTLOADER_H_ */
