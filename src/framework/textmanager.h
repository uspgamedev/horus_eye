#ifndef HORUSEYE_FRAMEWORK_TEXTMANAGER_H_
#define HORUSEYE_FRAMEWORK_TEXTMANAGER_H_

#include <vector>
#include <SDL/SDL_ttf.h>
#include "vector2D.h"

#define TEXT_MANAGER() framework::Engine::reference()->text_manager()

namespace framework {

using std::wstring;
class Text;
class Font;
class TextManager {
    public:
        TextManager() : current_font_(NULL) {}
        ~TextManager();

        bool Initialize();
        bool Release();

		Text* GetText(wstring text, wstring font = wstring(), int width = -1);
		Text* GetTextFromFile(wstring path, wstring font, int width = -1);
		void AddFont(wstring name, wstring path, int size, char ident, bool fancy);

    private:
		Font *current_font_;
		std::map<wstring,Image**> font_images_;
		std::map<wstring,Font*> fonts_;
};

} // namespace framework

#endif
