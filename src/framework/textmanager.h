#ifndef HORUSEYE_FRAMEWORK_TEXTMANAGER_H_
#define HORUSEYE_FRAMEWORK_TEXTMANAGER_H_

#include <vector>
#include <SDL/SDL_ttf.h>
#include "vector2D.h"

#define TEXT_MANAGER() framework::Engine::reference()->text_manager()

namespace framework {

class Image;
class TextManager{
    public:
        TextManager() : font_(NULL) {}
        ~TextManager() {}

        bool Initialize();
        bool Destroy();
        bool setFont(string font, int fontsize, string *style);
        bool setColor(int r, int g, int b);
        TTF_Font* getFont();

        Image* LoadFancyLine(string line);
        Image* LoadText(string text, char indent);
        Image* LoadFile(string path, char indent);

    private:
        TTF_Font *font_;
        SDL_Color textColor_;
        SDL_Color transparentColor_;
};

} // namespace framework

#endif
