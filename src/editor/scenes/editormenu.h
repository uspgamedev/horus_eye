#ifndef HORUSEYE_GAME_SCENES_EDITORMENUBUILDER_H_
#define HORUSEYE_GAME_SCENES_EDITORMENUBUILDER_H_

#include "game/scenes/menuhandler.h"
#include "game/scenes/menubuilder.h"
#include <vector>
#include <ugdk/graphic/node.h>

namespace ugdk {
class ugdk::graphic::Drawable;
class Image;
}

namespace utils {
class Settings;
}

namespace editor {
class MapEditor;
/*TODO: "menu" do editor, parecido com o Pause do world, porem com mais opcoes: (pelo menos essas)
		-Close menu (voltar pro editor)
		-New Map
		-Load Map
		-Save Map
		-Quit without saving
		-Save and quit
		
		lembrando q ai o ESC abriria esse menu de "pause" em vez de sair do editor direto.
		
		talvez alguma opcao ou grafico nele pra mostrar os comando de teclado...*/

class EditorMenuBuilder : scene::MenuBuilder {
  public:
    EditorMenuBuilder () {}
    virtual ~EditorMenuBuilder () {}
	/*List of Build Functions: creates each menu*/
    scene::Menu *BuildEditorMenu(MapEditor* editor);
    scene::Menu *BuildNewMapMenu();
	scene::Menu *BuildLoadMapMenu(MapEditor* editor);

  protected:
	/*List of Editor Menu enumerations definitions*/
    typedef enum {
        EDITOR_SELECT_CONTINUE = 0,
        EDITOR_SELECT_NEW_MAP,
		EDITOR_SELECT_LOAD_MAP,
		EDITOR_SELECT_SAVE_MAP,
		EDITOR_SELECT_QUIT,
		EDITOR_SELECT_SAVE_AND_QUIT,
        EDITOR_SELECT_NUM
    } EditorMenuSelection;
	
	typedef enum {
		LOAD_MAP_SELECT_CHANGE = 0,
		LOAD_MAP_SELECT_BLANK1,
		LOAD_MAP_SELECT_BLANK2,
		LOAD_MAP_SELECT_LOAD,
		LOAD_MAP_SELECT_BACK,
		LOAD_MAP_SELECT_NUM
	} LoadMapMenuSelection;

	/*List of Editor Menu Handler classes*/
	class EditorMenuHandler : public scene::MenuHandler {
      public:
        EditorMenuHandler(scene::Menu *menu, MapEditor* editor) : 
			scene::MenuHandler(menu), editor_(editor) {}
        ~EditorMenuHandler() {}
        void Handle(int selection, int modifier = 0);
        void CleanUp();
	  private:
		MapEditor* editor_;
    };

	class LoadMapMenuHandler : public scene::MenuHandler {
      public:
        LoadMapMenuHandler(scene::Menu *menu, MapEditor* editor);
        ~LoadMapMenuHandler() {}
        void Handle(int selection, int modifier = 0);
        void CleanUp();
		void BuildSprites();
	  private:
		MapEditor* editor_;
		std::vector<std::string> map_list_;
		ugdk::graphic::Node **level_nodes_;
		int selected_level_;
    };
};

}

#endif /* HORUSEYE_GAME_SCENES_EDITORMENUBUILDER_H_ */
