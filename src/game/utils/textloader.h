#ifndef HORUSEYE_GAME_UTILS_TEXTLOADER_H_
#define HORUSEYE_GAME_UTILS_TEXTLOADER_H_

#include <string>
#include <map>

namespace framework {
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
    framework::Image* GetImage(std::string text);

    bool Clear();

  private:
    TextLoader();
    std::map<std::string, framework::Image*> text_images_;
};

}

#endif /* HORUSEYE_GAME_UTILS_TEXTLOADER_H_ */
