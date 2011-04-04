#include <sstream>
#include "editormenu.h"
#include "../../framework/engine.h"
#include "../../framework/textmanager.h"
#include "../../framework/scene.h"
#include "../../framework/animation.h"
#include "../../game/utils/levelmanager.h"
#include "../../game/utils/imagefactory.h"
#include "../../game/utils/hudimagefactory.h"
#include "../../game/utils/textloader.h"
#include "../../game/utils/settings.h"
#include "../../game/utils/constants.h"
#include "../mapeditor.h"
#include "../../game/scenes/world.h"
#include "../../game/scenes/menuhandler.h"
#include "../../game/scenes/menu.h"
#include "../../game/scenes/page.h"

namespace editor {

using namespace framework;
using namespace utils;
using namespace std;

#define RECT_WIDTH              260
#define RECT_HEIGHT             50
#define SET_RECT_WIDTH          266
#define SET_RECT_HEIGHT         70
#define SELECTION_WIDTH         864
#define SELECTION_HEIGHT        155

#define MENU_TOP                VIDEO_MANAGER()->video_size().y/2.0f - (EditorMenuBuilder::EDITOR_SELECT_NUM*RECT_HEIGHT/2.0f)
#define MENU_BOTTOM             MENU_TOP + EditorMenuBuilder::EDITOR_SELECT_NUM*RECT_HEIGHT
#define MENU_LEFT               VIDEO_MANAGER()->video_size().x/2.0f - RECT_WIDTH/2.0f
#define MENU_RIGHT              MENU_LEFT + RECT_WIDTH


//========================
//   Editor Menu

scene::Menu *EditorMenuBuilder::BuildEditorMenu (MapEditor* editor) {

    // Our main menu.
	scene::Menu *menu = new scene::Menu(EditorMenuBuilder::EDITOR_SELECT_NUM);

    // Setting its handler.
	Image *bg_img = new Image;
    bg_img->Create(VIDEO_MANAGER()->video_size(), SDL_HWSURFACE);
    bg_img->Clear(0x808080);
    bg_img->SetAlpha(128);

    menu->set_handler(new EditorMenuHandler(menu, editor, bg_img));

    // The menu's content box.
    menu->set_content_box(Frame(MENU_LEFT, MENU_TOP, MENU_RIGHT, MENU_BOTTOM));

    // Setting the selection sprite.
    CreateSelectionSprites(menu, (MENU_BOTTOM-MENU_TOP)/EditorMenuBuilder::EDITOR_SELECT_NUM);

    Sprite *bg = new Sprite;;
    bg->Initialize(bg_img);
    menu->AddSprite(bg, Vector2D());

    // The sprite of each option.
    for (int i = 0; i < EditorMenuBuilder::EDITOR_SELECT_NUM; ++i) {
        Sprite *options_sprite = new Sprite;
        switch (i) {
        case EditorMenuBuilder::EDITOR_SELECT_CONTINUE:
            options_sprite->Initialize(TEXT_LOADER()->GetImage("Continue"));
            break;
        case EditorMenuBuilder::EDITOR_SELECT_NEW_MAP:
            options_sprite->Initialize(TEXT_LOADER()->GetImage("New"));
            break;
        case EditorMenuBuilder::EDITOR_SELECT_LOAD_MAP:
            options_sprite->Initialize(TEXT_LOADER()->GetImage("Load"));
            break;
        case EditorMenuBuilder::EDITOR_SELECT_SAVE_MAP:
            options_sprite->Initialize(TEXT_LOADER()->GetImage("Save"));
            break;
        case EditorMenuBuilder::EDITOR_SELECT_QUIT:
            options_sprite->Initialize(TEXT_LOADER()->GetImage("Quit"));
            break;
        case EditorMenuBuilder::EDITOR_SELECT_SAVE_AND_QUIT:
            options_sprite->Initialize(TEXT_LOADER()->GetImage("SaveQuit"));
            break;
        }
		options_sprite->set_hotspot(Vector2D(options_sprite->size().x/2, 0));
        menu->set_option_sprite(i, options_sprite);
    }

    return menu;
}

void EditorMenuBuilder::EditorMenuHandler::Handle(int selection, int modifier) {
    if (modifier) return;
    switch (selection) {
        case EditorMenuBuilder::EDITOR_SELECT_CONTINUE: {
			menu_->Finish();
            break;
        }
        case EditorMenuBuilder::EDITOR_SELECT_NEW_MAP: {
			
            break;
        }
        case EditorMenuBuilder::EDITOR_SELECT_LOAD_MAP: {
			string level = "data/levels/level_3.txt";
			editor_->LoadMap(level);
			menu_->Finish();
            break;
        }
        case EditorMenuBuilder::EDITOR_SELECT_SAVE_MAP: {
            if (editor_)
				editor_->SaveMap();
            break;
        }
        case EditorMenuBuilder::EDITOR_SELECT_QUIT: {
			menu_->Finish();
			editor_->Finish();
            break;
        }
        case EditorMenuBuilder::EDITOR_SELECT_SAVE_AND_QUIT: {
            if (editor_)
				editor_->SaveMap();
			menu_->Finish();
			editor_->Finish();
            break;
        }
        default: {
            break;
        }
    }
}

void EditorMenuBuilder::EditorMenuHandler::CleanUp() {
    bg_img_->Destroy();
    delete bg_img_;
}

}