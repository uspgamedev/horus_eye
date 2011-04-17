#ifndef HORUSEYE_FRAMEWORK_TEXTMANAGER_H_
#define HORUSEYE_FRAMEWORK_TEXTMANAGER_H_

#include <vector>
#include <SDL/SDL_ttf.h>
#include "vector2D.h"

#define TEXT_MANAGER() framework::Engine::reference()->text_manager()

namespace framework {

class Text;
class Font;
class TextManager {
    public:
        TextManager() : current_font_(NULL) {}
        ~TextManager() {}

        bool Initialize();
        bool Destroy();

		Text* GetText(string text, string font = string(), int width = -1);
		Text* GetTextFromFile(string path, string font, int width = -1);
		void AddFont(string name, string path, int size, char ident, bool fancy);

    private:
		Font *current_font_;
		std::map<string,Image**> font_images_;
		std::map<string,Font*> fonts_;
};

} // namespace framework

#endif
