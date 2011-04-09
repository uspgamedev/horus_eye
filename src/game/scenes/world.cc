#include <cmath>
#include <iostream>
#include "../../framework/engine.h"
#include "../../framework/videomanager.h"
#include "../../framework/scene.h"
#include "../../framework/vector2D.h"
#include "../../framework/inputmanager.h"
#include "../../framework/music.h"
#include "../../framework/audiomanager.h"
#include "world.h"
#include "imagescene.h"
#include "menu.h"
#include "menubuilder.h"
#include "../sprites/worldobject.h"
#include "../sprites/hero.h"
#include "../utils/hud.h"
#include "../utils/levelmanager.h"
#include "../utils/imagefactory.h"
#include "../utils/settings.h"
namespace scene {

using namespace framework;
using namespace sprite;
using namespace utils;
using namespace std;

World::World(sprite::Hero *hero) : Scene(), world_layer_(new framework::Layer()), music_(NULL) {
    world_layer_->set_light_type(LIGHT_ILLUMINATED);
    AddLayer(world_layer_);

	image_factory_ = new utils::ImageFactory();

    hero_ = hero;

    hud_ = new utils::Hud(this);
    AddLayer(hud_);

    remaining_enemies_ = max_enemies_ = 0;
    level_state_ = LevelManager::NOT_FINISHED;
    player_exit_ = false;
	konami_timeout_ = 0;
}

// Destrutor
World::~World() {
}

bool worldObjectIsDead (const WorldObject* value) {
    bool is_dead = ((*value).status() == WorldObject::STATUS_DEAD);
    if (is_dead) {
        delete value;
    }
    return is_dead;
}

bool World::verifyCollision(WorldObject *obj1, WorldObject *obj2) {
    if (obj1 == obj2) {
        return false;
    }
    if (obj2->collision_type() == WorldObject::NO_COLLISION) {
        return false;
    }

    return obj1->IsColliding(obj2);
}

void World::HandleCollisions() {
    std::list<sprite::WorldObject*>::iterator i, j;

    // TODO: colisao esta sendo verificada 2x por iteracao, corrigir isso
    for (i = world_objects_.begin(); i != world_objects_.end(); ++i) {
        if ((*i)->collision_type() == WorldObject::MOVEABLE) {
            for (j = world_objects_.begin(); j != world_objects_.end(); ++j) {
                if (verifyCollision(*i, *j)) {
                    (*i)->HandleCollision(*j);
                    (*j)->HandleCollision(*i);
                }
            }
        }
    }
}

void World::VerifyCheats(float delta_t) {
    InputManager *input = Engine::reference()->input_manager();

    if (input->KeyPressed(K_p)) {
        LevelManager *level_manager = LevelManager::reference();
        level_manager->SetNextLevel(level_manager->GetNextLevelID() + 1);
        level_state_ = LevelManager::FINISH_WARP;
    } else if (input->KeyPressed(K_o)) {
        LevelManager *level_manager = LevelManager::reference();
        unsigned int cur_level = level_manager->GetNextLevelID();
        if(cur_level > 0) {
            level_manager->SetNextLevel(cur_level - 1);
            level_state_ = LevelManager::FINISH_WARP;
        }
    }
    if(input->KeyPressed(K_h)) {
        hero_->set_life(hero_->max_life());
        hero_->set_mana(hero_->max_mana());
    }
    if(input->KeyPressed(K_t))
        hero_->set_world_position(FromScreenCoordinates(input->GetMousePosition()));

	if(konami_timeout_ >= 0.0f) {
		konami_timeout_ += delta_t;
		if(konami_timeout_ > 1.5f) {
			konami_buffer_.clear();
		}
		if(input->KeyPressed(K_UP)) {
			konami_buffer_.append("U");
			konami_timeout_ = 0.0f;
		}
		if(input->KeyPressed(K_DOWN)) {
			konami_buffer_.append("D");
			konami_timeout_ = 0.0f;
		}
		if(input->KeyPressed(K_LEFT)) {
			konami_buffer_.append("L");
			konami_timeout_ = 0.0f;
		}
		if(input->KeyPressed(K_RIGHT)) {
			konami_buffer_.append("R");
			konami_timeout_ = 0.0f;
		}
		if(input->KeyPressed(K_a)) {
			konami_buffer_.append("a");
			konami_timeout_ = 0.0f;
		}
		if(input->KeyPressed(K_b)) {
			konami_buffer_.append("b");
			konami_timeout_ = 0.0f;
		}
		if(konami_buffer_.compare("UUDDLRLRba") == 0) {
			hero_->Invulnerable(85000);
			AUDIO_MANAGER()->LoadMusic("data/musics/sf2Guile456.mid")->Play();
			konami_timeout_ = -1;
		}
	}
}

Vector2D World::ActualOffset() {
    Vector2D result = Vector2D(0,0)-VIDEO_MANAGER()->video_size()*0.5;
    if(hero_) result = result + hero_->position();

    return result;
}

bool World::VerifyPause() {
    InputManager *input = Engine::reference()->input_manager();
    if(input->KeyPressed(K_ESCAPE)) {
        MenuBuilder builder;
        Engine::reference()->PushScene(builder.BuildPauseMenu());
        return true;
    }
    return false;
}

void World::Start() {
	if(music_ != NULL)
		music_->PlayForever();
}

void World::Update(float delta_t) {

    if(VerifyPause()) return;

    set_visible(true);
    Scene::Update(delta_t);

    HandleCollisions();

    RemoveInactiveObjects();
    AddNewWorldObjects();
    
    world_layer_->set_offset(ActualOffset());
	//TODO: re-implementar isso!
    //???->UpdateVisibility();

	if (!hero_)
        level_state_ = LevelManager::FINISH_DIE;

//#ifdef DEBUG
    VerifyCheats(delta_t);
//#endif
    if (level_state_ != LevelManager::NOT_FINISHED)
        LevelManager::reference()->FinishLevel(level_state_);

}

void World::End() {
	if(music_ != NULL)
		music_->Stop();
	if(hero_ != NULL)
		hero_->Invulnerable(0);
    this->RemoveAll();
    for (int i = 0; i < (int)level_matrix_.size(); i++)
        for (int j = 0; j < (int)level_matrix_[i].size(); j++)
            delete level_matrix_[i][j];
}

void World::IncreaseNumberOfEnemies() {
    remaining_enemies_++;
    max_enemies_++;
}

void World::AddWorldObject(sprite::WorldObject* new_object, framework::Vector2D pos) {

    new_object-> set_world_position(pos);
    new_world_objects.push_front(new_object);
}

void World::AddNewWorldObjects() {
    for (list<sprite::WorldObject*>::iterator it = new_world_objects.begin();
         it != new_world_objects.end();
         ++it) {

        WorldObject *new_object = *it;
        if(new_object->collision_type() == WorldObject::NO_COLLISION) {
            collisionless_objects.push_front(new_object);
        } else {
            world_objects_.push_front(new_object);
        }

        world_layer_->AddSprite(new_object);
    }
    new_world_objects.clear();
}

void World::AddHero(framework::Vector2D pos) {
    this->AddWorldObject(hero_, pos);
}

int World::CountRemainingEnemies() {
    return 	remaining_enemies_;
}

void World::RemoveInactiveObjects() {
    std::list<sprite::WorldObject*>::iterator i;
    if(hero_ != NULL && hero_->status() == WorldObject::STATUS_DEAD) {
        hero_ = NULL;
    }
    for (i = world_objects_.begin(); i != world_objects_.end(); ++i) {
        if((*i)->status() == WorldObject::STATUS_DEAD) {
            world_layer_->RemoveSprite(*i);
        }
    }
    world_objects_.remove_if(worldObjectIsDead);
}

void World::RemoveAll() {

    std::list<sprite::WorldObject*>::iterator i;
    for (i = world_objects_.begin(); i != world_objects_.end(); ++i) {
        world_layer_->RemoveSprite(*i);
        if ( *i != hero_ ) {
            delete (*i);
        }
    }
    world_objects_.clear();
    for (i = collisionless_objects.begin(); i != collisionless_objects.end(); ++i) {
        world_layer_->RemoveSprite(*i);
        delete (*i);
    }
    collisionless_objects.clear();
    hero_ = NULL;

}

Vector2D World::FromScreenLinearCoordinates(Vector2D screen_coords) {
    Vector2D tx(sqrt(5.0)/4.0f, -sqrt(5.0)/4.0f);
    Vector2D ty(-sqrt(5.0)/2.0f, -sqrt(5.0)/2.0f);
    return (tx * screen_coords.x) + (ty * screen_coords.y);
}

Vector2D World::FromWorldLinearCoordinates(Vector2D world_coords) {
    Vector2D tx(54.0f, -27.0f);
    Vector2D ty(-54.0f, -27.0f);
    return (tx * world_coords.x) + (ty * world_coords.y);
}

Vector2D World::FromWorldCoordinates(Vector2D world_coords) {
    return FromWorldLinearCoordinates(world_coords);
}

Vector2D World::FromScreenCoordinates(Vector2D screen_coords) {
    Vector2D    global_screen_coords = screen_coords + WORLD()->world_layer_->offset(),
                transformed = FromScreenLinearCoordinates(global_screen_coords);
    return (transformed * (1.0f/60.373835392f));
}

const Vector2D World::ConvertLightRadius(float radius) {
    Vector2D ellipse_coords = Vector2D(2, 1) * radius * 60.373835392;
    return ellipse_coords;
}

void World::set_music(std::string &music) {
	if(music_ != NULL && music_->IsPlaying()) {
		music_->Stop();
	}
	utils::Settings settings;
	music_ = settings.background_music()
		? AUDIO_MANAGER()->LoadMusic(music)
		: NULL;
}

} // namespace scene
