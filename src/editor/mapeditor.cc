#include <fstream>
#include <iostream>
#include <cmath>
#include <queue>
#include <ugdk/base/engine.h>
#include <ugdk/input/inputmanager.h>
#include "mapeditor.h"
#include "mapobject.h"
#include "layers/maptileslayer.h"
#include "layers/mapspriteslayer.h"
#include "layers/fpsmeter.h"
#include "scenes/editormenu.h"
#include "../game/utils/levelmanager.h"

using namespace std;
using namespace ugdk;

namespace editor {

MapEditor::MapEditor() : Scene() {
	map_loaded_ = false;
    main_layer_ = tiles_layer_ = new MapTilesLayer(&map_matrix_, this);
    AddLayer(tiles_layer_);
	fps_layer_ = new FPSMeter;
    selected_object_ = NULL;

	utils::LevelManager::reference()->LoadLevelList("data/level_list.txt", map_list_);
}

MapEditor::~MapEditor() {
}

void MapEditor::LoadMap(std::string& file_name) {
	if (map_loaded_) {
		RemoveLayer(sprites_layer_);
		delete sprites_layer_;
		RemoveLayer(fps_layer_);
	}
	sprites_layer_ = new MapSpritesLayer(this);
	sprites_layer_->set_visible(false);
	AddLayer(sprites_layer_);
	AddLayer(fps_layer_);
	tiles_layer_->set_visible(true);
	main_layer_ = tiles_layer_;

	map_filename_ = file_name;
	ifstream file (file_name.c_str());
    if(file.is_open()){
		file >> music_;
        file >> width_ >> height_;
        vector<string> raw_matrix (height_);
        map_matrix_ = vector< vector<MapObject*> >(height_);

        for (int i = 0; i < height_; ++i) {
            file >> raw_matrix[i];
            map_matrix_[i] = vector<MapObject*>(width_);
            for (int j = 0; j < width_; j++) {
                map_matrix_[i][j] = new MapObject(i, j, raw_matrix[i][j], width_, height_);
            }
        }
        file.close();
		map_loaded_ = true;
		sprites_layer_->LoadMapMatrix(&map_matrix_);
		offset_ = Vector2D(width_ * 0.5f, height_ * 0.5f);
		scale_level_ = 0;
    } else {
        cout << "CANNOT OPEN FILE: " << file_name << endl;
		map_loaded_ = false;
        exit(0);
    }
}

void MapEditor::Update(float delta_t) {
    Scene::Update(delta_t);

    InputManager *input = ugdk::Engine::reference()->input_manager();
    if(input->KeyPressed(ugdk::K_ESCAPE) || map_loaded_ == false) {
		EditorMenuBuilder builder;
        Engine::reference()->PushScene(builder.BuildEditorMenu(this));
	}
	if (!map_loaded_) return;
	
	if(input->KeyPressed(ugdk::K_MINUS))
        scale_level_--;
	else if(input->KeyPressed(ugdk::K_EQUALS))
        scale_level_++;

    main_layer_->set_scale(exp(scale_level_ * 0.10f));

    Vector2D movement;

    if(input->MousePressed(ugdk::M_BUTTON_RIGHT)) {
        drag_click_ = false;
        click_start_position_ = input->GetMousePosition();
    }

    if(input->MouseDown(ugdk::M_BUTTON_RIGHT)) {
        movement = (input->GetMousePosition() - last_mouse_position_);
        if((click_start_position_ - input->GetMousePosition()).length() > 10.0f)
            drag_click_ = true;

    } else {
        if(input->KeyDown(ugdk::K_LEFT))
            movement.x -= 100.0f * delta_t;
        if(input->KeyDown(ugdk::K_RIGHT))
            movement.x += 100.0f * delta_t;
        if(input->KeyDown(ugdk::K_UP))
            movement.y -= 100.0f * delta_t;
        if(input->KeyDown(ugdk::K_DOWN))
            movement.y += 100.0f * delta_t;
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
    if(input->MouseDown(ugdk::M_BUTTON_LEFT) && !drag_click_) {
		MapObject* new_obj = main_layer_->Select(last_mouse_position_);
        if(selected_object_)
            selected_object_->Select(false);
        selected_object_ = (new_obj == selected_object_ && input->MousePressed(ugdk::M_BUTTON_LEFT)) ? NULL : new_obj;
        if(selected_object_)
            selected_object_->Select(true);
    }

	this->processKeyEditCommands();

    if(input->KeyPressed(ugdk::K_END)) {
        main_layer_->set_visible(false);
        if(main_layer_ == tiles_layer_) {
            main_layer_ = sprites_layer_;
            //this->RemoveLayer(tiles_layer_);
            //this->AddLayer(main_layer_ = sprites_layer_);
        } else {
            main_layer_ = tiles_layer_;
            //this->RemoveLayer(sprites_layer_);
            //this->AddLayer(main_layer_ = tiles_layer_);
        }
        main_layer_->set_visible(true);
    }
	else if (input->KeyPressed(ugdk::K_HOME)) {
		this->SaveMap();
	}

}

void MapEditor::processKeyEditCommands() {
	InputManager *input = ugdk::Engine::reference()->input_manager();

	char type = '-';
	bool doFill = false;

	/*object modifier keys*/
	bool isStanding = !(input->KeyDown(ugdk::K_LSHIFT) || input->KeyDown(ugdk::K_RSHIFT));

	/*basic object keys*/
	if(input->KeyDown(ugdk::K_w)) type = WALL;
	if(input->KeyDown(ugdk::K_d)) type = DOOR;
	if(input->KeyDown(ugdk::K_e)) type = ENTRY;
	if(input->KeyDown(ugdk::K_m)) type = (isStanding) ? STANDING_MUMMY : MUMMY;
	if(input->KeyDown(ugdk::K_r)) type = (isStanding) ? STANDING_RANGED_MUMMY : RANGED_MUMMY;
	if(input->KeyDown(ugdk::K_b)) type = (isStanding) ? STANDING_BIG_MUMMY : BIG_MUMMY;
	if(input->KeyDown(ugdk::K_p)) type = (isStanding) ? STANDING_PHARAOH : PHARAOH;
	if(input->KeyDown(ugdk::K_h)) type = HERO;
	if(input->KeyDown(ugdk::K_x)) type = FLOOR;
	if(input->KeyDown(ugdk::K_o)) type = EMPTY;
	if(input->KeyDown(ugdk::K_l)) type = POTIONL;
	if(input->KeyDown(ugdk::K_n)) type = POTIONM;
	if(input->KeyDown(ugdk::K_s)) type = POTIONS;	

	/*advanced commands keys*/
	if(input->KeyDown(ugdk::K_LCTRL) || input->KeyDown(ugdk::K_RCTRL)) doFill = true;

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
			sprites_layer_->RemoveSprite(aux);
			delete aux;
			map_matrix_[i][j] = new MapObject(i, j, type, width_, height_);
			sprites_layer_->AddSprite(map_matrix_[i][j]);
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
