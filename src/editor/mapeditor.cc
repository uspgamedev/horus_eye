#include <fstream>
#include <iostream>
#include <cmath>
#include <queue>
#include <algorithm>
#include <ugdk/base/engine.h>
#include <ugdk/input/inputmanager.h>
#include "mapeditor.h"
#include "mapobject.h"
#include "layers/maptileslayer.h"
#include "layers/mapspriteslayer.h"
#include "scenes/editormenu.h"
#include "game/scenes/menu.h"
#include "game/utils/levelmanager.h"
#include "game/utils/tile.h"

using std::string;
using std::vector;
using std::queue;
using std::ofstream;
using std::cout;
using std::endl;

using ugdk::Engine;
using ugdk::Vector2D;

#define LINE_SIZE 1024

/* Util functions found at http://stackoverflow.com/q/217605 */
// trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(isspace))));
    return s;
}
// trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(isspace))).base(), s.end());
    return s;
}
// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

namespace editor {

MapEditor::MapEditor() : Scene() {
	// No map has been loaded yet
	map_loaded_ = false;

    main_layer_ = tiles_layer_ = new MapTilesLayer(this);
	sprites_layer_ = new MapSpritesLayer(this);
	
	this->interface_node()->AddChild(tiles_layer_->node());
	this->interface_node()->AddChild(sprites_layer_->node());

	sprites_layer_->node()->modifier()->set_visible(false);

	//fps_layer_ = new FPSMeter; // TODO
    selected_object_ = NULL;

	utils::LevelManager::reference()->LoadLevelList("level_list.txt", map_list_);
}

MapEditor::~MapEditor() {
}

void MapEditor::End() {
	this->interface_node()->RemoveChild(tiles_layer_->node());
	this->interface_node()->RemoveChild(sprites_layer_->node());

    if (map_loaded_) {
        //Engine::reference()->RemoveInterface(fps_layer_);
    }

    delete tiles_layer_;
    delete sprites_layer_;
    //delete fps_layer_;
}

void MapEditor::LoadMap(std::string& file_name) {
	map_filename_ = file_name;
    FILE *file = fopen(file_name.c_str(), "r");

    if(file != NULL) {
        char buffer[LINE_SIZE];
        fgets(buffer, LINE_SIZE, file);
		music_ = string(buffer);
        trim(music_);

        fgets(buffer, LINE_SIZE, file);
        sscanf(buffer, "%d %d", &width_, &height_);

        map_matrix_ = vector< vector<MapObject*> >(height_);

        for (int i = 0; i < height_; ++i) {
            fgets(buffer, LINE_SIZE, file);
            map_matrix_[i] = vector<MapObject*>(width_);
            for (int j = 0; j < width_; j++) {
                map_matrix_[i][j] = new MapObject(i, j, buffer[j], width_, height_);
            }
        }
        fclose(file);
		map_loaded_ = true;
		sprites_layer_->LoadMapMatrix(&map_matrix_);
		tiles_layer_->LoadMapMatrix(&map_matrix_);
		offset_ = Vector2D(width_ * 0.5, height_ * 0.5);
		scale_level_ = 0;
    } else {
        cout << "CANNOT OPEN FILE: " << file_name << endl;
		map_loaded_ = false;
        exit(0);
    }
}

void MapEditor::Update(double delta_t) {
    Scene::Update(delta_t);

    ugdk::input::InputManager *input = INPUT_MANAGER();
	// When the user presses ESCAPE (or on start), the EditorMenu is created.
    if(input->KeyPressed(ugdk::input::K_ESCAPE) || map_loaded_ == false) {
		EditorMenuBuilder builder;
		scene::Menu* editor_menu = builder.BuildEditorMenu(this);
        Engine::reference()->PushScene(editor_menu);
	}
	if (!map_loaded_) return;
	
	if(input->KeyPressed(ugdk::input::K_MINUS))
        scale_level_--;
	else if(input->KeyPressed(ugdk::input::K_EQUALS))
        scale_level_++;

	double scale = exp(scale_level_ * 0.10);
	main_layer_->node()->modifier()->set_scale(Vector2D(scale));

    Vector2D movement;

    if(input->MousePressed(ugdk::input::M_BUTTON_RIGHT)) {
        drag_click_ = false;
        click_start_position_ = input->GetMousePosition();
    }

    if(input->MouseDown(ugdk::input::M_BUTTON_RIGHT)) {
        movement = (input->GetMousePosition() - last_mouse_position_);
        if((click_start_position_ - input->GetMousePosition()).length() > 10.0)
            drag_click_ = true;

    } else {
        if(input->KeyDown(ugdk::input::K_LEFT))
            movement.x -= 100.0 * delta_t;
        if(input->KeyDown(ugdk::input::K_RIGHT))
            movement.x += 100.0 * delta_t;
        if(input->KeyDown(ugdk::input::K_UP))
            movement.y -= 100.0 * delta_t;
        if(input->KeyDown(ugdk::input::K_DOWN))
            movement.y += 100.0 * delta_t;
		drag_click_ = false;
    }
    offset_ = offset_ - main_layer_->ModifyMovement(movement);
	if (offset_.x < 0) offset_.x = 0;
	if (offset_.y < 0) offset_.y = 0;
	if (offset_.x > width_) offset_.x = width_;
	if (offset_.y > height_) offset_.y = height_;

    tiles_layer_->CenterAt(offset_);
    sprites_layer_->CenterAt(offset_);

    last_mouse_position_ = input->GetMousePosition();
    if(input->MouseDown(ugdk::input::M_BUTTON_LEFT) && !drag_click_) {
		MapObject* new_obj = main_layer_->Select(last_mouse_position_);
        if(selected_object_)
            selected_object_->Select(false);
        selected_object_ = (new_obj == selected_object_ && input->MousePressed(ugdk::input::M_BUTTON_LEFT)) ? NULL : new_obj;
        if(selected_object_)
            selected_object_->Select(true);
    }

	this->processKeyEditCommands();

    if(input->KeyPressed(ugdk::input::K_END)) {
        main_layer_->node()->modifier()->set_visible(false);
        if(main_layer_ == tiles_layer_) {
            main_layer_ = sprites_layer_;
        } else {
            main_layer_ = tiles_layer_;
        }
        main_layer_->node()->modifier()->set_visible(true);
    }
	else if (input->KeyPressed(ugdk::input::K_HOME)) {
		this->SaveMap();
	}

}

void MapEditor::processKeyEditCommands() {
    ugdk::input::InputManager *input = INPUT_MANAGER();

	char type = '-';
	bool doFill = false;

	/*object modifier keys*/
	bool isStanding = !(input->KeyDown(ugdk::input::K_LSHIFT) || input->KeyDown(ugdk::input::K_RSHIFT));

	/*basic object keys*/
	if(input->KeyDown(ugdk::input::K_w)) type = WALL;
	if(input->KeyDown(ugdk::input::K_d)) type = DOOR;
	if(input->KeyDown(ugdk::input::K_e)) type = ENTRY;
	if(input->KeyDown(ugdk::input::K_m)) type = (isStanding) ? STANDING_MUMMY : MUMMY;
	if(input->KeyDown(ugdk::input::K_r)) type = (isStanding) ? STANDING_RANGED_MUMMY : RANGED_MUMMY;
	if(input->KeyDown(ugdk::input::K_b)) type = (isStanding) ? STANDING_BIG_MUMMY : BIG_MUMMY;
	if(input->KeyDown(ugdk::input::K_p)) type = (isStanding) ? STANDING_PHARAOH : PHARAOH;
	if(input->KeyDown(ugdk::input::K_h)) type = HERO;
	if(input->KeyDown(ugdk::input::K_x)) type = FLOOR;
	if(input->KeyDown(ugdk::input::K_o)) type = EMPTY;
	if(input->KeyDown(ugdk::input::K_l)) type = POTIONL;
	if(input->KeyDown(ugdk::input::K_n)) type = POTIONM;
	if(input->KeyDown(ugdk::input::K_s)) type = POTIONS;
    if(input->KeyDown(ugdk::input::K_k)) type = BLOCK;
    if(input->KeyDown(ugdk::input::K_u)) type = BUTTON;

	/*advanced commands keys*/
	if(input->KeyDown(ugdk::input::K_LCTRL) || input->KeyDown(ugdk::input::K_RCTRL)) doFill = true;

	if (selected_object_ && selected_object_->type() != type && type != '-') {
		int i, j, selI, selJ, a, b;
		selI = selected_object_->y();
		selJ = selected_object_->x();
		char c = selected_object_->type();
		MapObject* aux;

		selected_object_->Select(false);

		queue<MapObject*> q;
		q.push(selected_object_);


		while (!q.empty()) {
			aux = q.front();
			q.pop();

			i = aux->y();
			j = aux->x();

			// Removing the MapObject
			sprites_layer_->node()->RemoveChild(aux->isometric_node());
			tiles_layer_->node()->RemoveChild(aux->tile_node());
			delete aux;

			// Creatings a new
			map_matrix_[i][j] = new MapObject(i, j, type, width_, height_);

			// Adding it to the screen
			sprites_layer_->node()->AddChild(map_matrix_[i][j]->isometric_node());
			tiles_layer_->node()->AddChild(map_matrix_[i][j]->tile_node());

			map_matrix_[i][j]->set_is_in_fill(false);

			if (doFill) {
				for (a=-1; a < 2; a++)
					if (i+a >= 0 && i+a < height_)
						for (b=-1; b<2; b++)
							if (j+b >= 0 && j+b < width_ && (i+a == i || j+b == j))
								if (map_matrix_[i+a][j+b]->type() == c && !map_matrix_[i+a][j+b]->is_in_fill()) {
									q.push(map_matrix_[i+a][j+b]);
									map_matrix_[i+a][j+b]->set_is_in_fill(true);
								}
			}
		}
		
		selected_object_ = map_matrix_[selI][selJ];
		selected_object_->Select(true);
	}
}

void MapEditor::SaveMap() {
	if (!map_loaded_) return;

	ofstream file (map_filename_.c_str());
    if(file.is_open()){
		file << music_ << "\n";
        file << width_ << " " << height_ << "\n";
        
        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; j++) {
                file << map_matrix_[i][j]->type();
            }
			file << "\n";
        }
        file.close();
    } else {
        cout << "CANNOT OPEN FILE: " << map_filename_ << endl;
    }
}


}
