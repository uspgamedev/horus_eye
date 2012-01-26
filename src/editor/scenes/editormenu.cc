#include <sstream>
#include "editormenu.h"
#include <ugdk/base/engine.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/action/scene.h>
#include <ugdk/action/animation.h>
#include <ugdk/graphic/drawable/solidrectangle.h>
#include <ugdk/graphic/drawable/text.h>
#include <ugdk/graphic/drawable.h>
#include "game/utils/levelmanager.h"
#include "game/utils/imagefactory.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/textloader.h"
#include "game/utils/settings.h"
#include "game/utils/constants.h"
#include "editor/mapeditor.h"
#include "game/scenes/world.h"
#include "game/scenes/menuhandler.h"
#include "game/scenes/menu.h"

namespace editor {

using namespace ugdk;
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

scene::Menu* EditorMenuBuilder::BuildEditorMenu (MapEditor* editor) {

    // Our main menu.
	scene::Menu *menu = new scene::Menu(EditorMenuBuilder::EDITOR_SELECT_NUM);

    // Setting its handler.
    menu->set_handler(new EditorMenuHandler(menu, editor));

    // The menu's content box.
    menu->set_content_box(Frame(MENU_LEFT, MENU_TOP, MENU_RIGHT, MENU_BOTTOM));

    // Setting the selection sprite.
    CreateSelectionSprites(menu, (MENU_BOTTOM-MENU_TOP)/EditorMenuBuilder::EDITOR_SELECT_NUM);

	// The background color.
    ugdk::SolidRectangle* bg = new ugdk::SolidRectangle(VIDEO_MANAGER()->video_size());
    bg->set_color(ugdk::Color(0.5f, 0.5f, 0.5f, 0.5f));
    menu->AddDrawable(bg, Vector2D());

    // The text of each option.
    for (int i = 0; i < EditorMenuBuilder::EDITOR_SELECT_NUM; ++i) {
		Drawable *options_sprite = NULL;
        switch (i) {
        case EditorMenuBuilder::EDITOR_SELECT_CONTINUE:
            options_sprite = TEXT_LOADER()->GetImage("Continue");
            break;
        case EditorMenuBuilder::EDITOR_SELECT_NEW_MAP:
            options_sprite = TEXT_LOADER()->GetImage("New");
            break;
        case EditorMenuBuilder::EDITOR_SELECT_LOAD_MAP:
            options_sprite = TEXT_LOADER()->GetImage("Load");
            break;
        case EditorMenuBuilder::EDITOR_SELECT_SAVE_MAP:
            options_sprite = TEXT_LOADER()->GetImage("Save");
            break;
        case EditorMenuBuilder::EDITOR_SELECT_QUIT:
            options_sprite = TEXT_LOADER()->GetImage("Quit");
            break;
        case EditorMenuBuilder::EDITOR_SELECT_SAVE_AND_QUIT:
            options_sprite = TEXT_LOADER()->GetImage("SaveQuit");
            break;
        }
        menu->set_option_sprite(i, options_sprite);
    }

    return menu;
}

void EditorMenuBuilder::EditorMenuHandler::Handle(int selection, int modifier) {
    if (modifier) return;
    switch (selection) {

		// Simply close this menu.
        case EditorMenuBuilder::EDITOR_SELECT_CONTINUE: {
			menu_->Finish();
            break;
        }

		// NOTHING HAHAHAHA. TODO
        case EditorMenuBuilder::EDITOR_SELECT_NEW_MAP: {
			
            break;
        }

		// Closes this menu and creates a level select menu.
        case EditorMenuBuilder::EDITOR_SELECT_LOAD_MAP: {
			EditorMenuBuilder builder;
			Engine::reference()->PushScene(builder.BuildLoadMapMenu(editor_));
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

void EditorMenuBuilder::EditorMenuHandler::CleanUp() {}


//========================
//   Load Map Menu

EditorMenuBuilder::LoadMapMenuHandler::LoadMapMenuHandler(scene::Menu *menu, MapEditor* editor) : 
			scene::MenuHandler(menu), editor_(editor) {
				map_list_ = editor->map_list();
				selected_level_ = 0;

				level_nodes_ = new ugdk::Node*[map_list_.size()];
			}

scene::Menu *EditorMenuBuilder::BuildLoadMapMenu (MapEditor* editor) {

    // Our main menu.
	scene::Menu *menu = new scene::Menu(EditorMenuBuilder::LOAD_MAP_SELECT_NUM);


	LoadMapMenuHandler* handler = new LoadMapMenuHandler(menu, editor);
    menu->set_handler(handler);

    // The menu's content box.
    menu->set_content_box(Frame(MENU_LEFT, MENU_TOP, MENU_RIGHT, MENU_BOTTOM));

    // Setting the selection sprite.
    CreateSelectionSprites(menu, (MENU_BOTTOM-MENU_TOP)/EditorMenuBuilder::LOAD_MAP_SELECT_NUM);

	ugdk::SolidRectangle* bg = new ugdk::SolidRectangle(VIDEO_MANAGER()->video_size());
    bg->set_color(ugdk::Color(0.25f, 0.25f, 0.25f, 0.8f));
    menu->AddDrawable(bg, Vector2D());

    // The sprite of each option.
    for (int i = 0; i < EditorMenuBuilder::LOAD_MAP_SELECT_NUM; ++i) {
        Drawable *options_sprite = NULL;
        switch (i) {
        case EditorMenuBuilder::LOAD_MAP_SELECT_CHANGE:
            options_sprite = TEXT_LOADER()->GetImage("Change");
            break;
        case EditorMenuBuilder::LOAD_MAP_SELECT_LOAD:
            options_sprite = TEXT_LOADER()->GetImage("Load");
            break;
        case EditorMenuBuilder::LOAD_MAP_SELECT_BACK:
			options_sprite = TEXT_LOADER()->GetImage("Back");
            break;
        }

		if (options_sprite != NULL) {
			menu->set_option_sprite(i, options_sprite);
		}
    }
	handler->BuildSprites();


    return menu;
}

void EditorMenuBuilder::LoadMapMenuHandler::Handle(int selection, int modifier) {
    if (!modifier) modifier = 1;
    switch (selection) {
        case EditorMenuBuilder::LOAD_MAP_SELECT_CHANGE: {
			level_nodes_[selected_level_]->modifier()->set_visible(false);
			selected_level_ += modifier;
			if (selected_level_ < 0) { selected_level_ = map_list_.size() - 1; }
			if (selected_level_ >= static_cast<int>(map_list_.size())) {
			    selected_level_ = 0;
			}
            level_nodes_[selected_level_]->modifier()->set_visible(true);
            break;
        }
        case EditorMenuBuilder::LOAD_MAP_SELECT_LOAD: {
			string level = map_list_[selected_level_];
			editor_->LoadMap(level);
			menu_->Finish();
            break;
        }
        case EditorMenuBuilder::LOAD_MAP_SELECT_BACK: {
			menu_->Finish();
            break;
        }
        default: {
            break;
        }
    }
}

void EditorMenuBuilder::LoadMapMenuHandler::BuildSprites() {
    // Creates the level images vector.
	for (size_t i = 0; i < map_list_.size(); ++i) {

		// Transform a std::string into a std::wstring (somehow)
        std::wstring tmpw(map_list_[i].length(), L' '); // Make room for characters
		std::copy(map_list_[i].begin(), map_list_[i].end(), tmpw.begin());


        Drawable* drawable = TEXT_MANAGER()->GetText(tmpw, L"FontB");
		drawable->set_hotspot(Drawable::CENTER);
		
		float x = VIDEO_MANAGER()->video_size().x * 0.5f;
		level_nodes_[i] = new Node(drawable);
		level_nodes_[i]->modifier()->set_offset(Vector2D(x, MENU_TOP + RECT_HEIGHT + 25.0f));
		menu_->AddNode(level_nodes_[i]);
        if ( static_cast<int>(i) != selected_level_ ) level_nodes_[i]->modifier()->set_visible(false);
    }
}
void EditorMenuBuilder::LoadMapMenuHandler::CleanUp() {
	delete[] level_nodes_;
}

}
