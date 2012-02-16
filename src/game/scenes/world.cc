#include <cmath>
#include <iostream>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/action/scene.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/audio/music.h>

#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionlogic.h>

#include "world.h"

#include "game/scenes/imagescene.h"
#include "game/scenes/menu.h"
#include "game/scenes/menubuilder.h"

#include "game/sprites/worldobject.h"
#include "game/sprites/creatures/hero.h"
#include "game/utils/hud.h"
#include "game/utils/levelmanager.h"
#include "game/utils/imagefactory.h"
#include "game/utils/settings.h"
#include "game/utils/visionstrategy.h"

namespace scene {

using namespace ugdk;
using namespace sprite;
using namespace utils;
using namespace std;
using pyramidworks::collision::CollisionInstance;

World::World(sprite::Hero *hero, utils::ImageFactory *factory) 
    :   Scene(),
        hero_(hero),
        world_node_(new ugdk::graphic::Node),
        remaining_enemies_(0),
        max_enemies_(0),
        image_factory_(factory),
        level_state_(LevelManager::NOT_FINISHED),
        konami_used_(false),
        lights_on_(true),
        num_button_not_pressed_(0) {

    content_node()->AddChild(world_node_);
    world_node_->modifier()->ToggleFlag(ugdk::graphic::Modifier::TRUNCATES_WHEN_APPLIED);

    hud_ = new utils::Hud(this);
	interface_node()->AddChild(hud_->node());
    this->AddEntity(hud_);
}

// Destrutor
World::~World() {
    if(image_factory_) delete image_factory_;
}

bool worldObjectIsDead (const WorldObject* value) {
    bool is_dead = ((*value).status() == WorldObject::STATUS_DEAD);
    if (is_dead) {
        delete value;
    }
    return is_dead;
}

void World::HandleCollisions() {
    std::list<CollisionInstance> collision_list;

    std::list<sprite::WorldObject*>::iterator i;
    for (i = colliding_world_objects_.begin(); i != colliding_world_objects_.end(); ++i)
        (*i)->collision_object()->SearchCollisions(collision_list);

    std::list<CollisionInstance>::iterator it;
    for(it = collision_list.begin(); it != collision_list.end(); ++it) {
        it->first->Handle(it->second);
    }
}

void World::VerifyCheats(double delta_t) {
    ugdk::input::InputManager *input = Engine::reference()->input_manager();

    if (input->KeyPressed(ugdk::input::K_p)) {
        LevelManager *level_manager = LevelManager::reference();
        level_manager->SetNextLevel(level_manager->GetNextLevelID() + 1);
        level_state_ = LevelManager::FINISH_WARP;
    } else if (input->KeyPressed(ugdk::input::K_o)) {
        LevelManager *level_manager = LevelManager::reference();
        unsigned int cur_level = level_manager->GetNextLevelID();
        if(cur_level > 0) {
            level_manager->SetNextLevel(cur_level - 1);
            level_state_ = LevelManager::FINISH_WARP;
        }
    }
    if(input->KeyPressed(ugdk::input::K_h)) {
        if(input->KeyDown(ugdk::input::K_LSHIFT))
            hero_->mana_blocks().Fill();
        hero_->life().Fill();
        hero_->mana().Fill();
    }
    if(input->KeyPressed(ugdk::input::K_t))
        hero_->set_world_position(FromScreenCoordinates(input->GetMousePosition()));

    if(input->KeyPressed(ugdk::input::K_l))
        VIDEO_MANAGER()->SetLightSystem(lights_on_ = !lights_on_);

	// EASTER EGG/TODO: remove before any release!
	// Also erase musics/sf2Guile456.mid
	/*if(!konami_used_) {
		Key konami[10] = { K_UP, K_UP, K_DOWN, K_DOWN, K_LEFT, K_RIGHT, K_LEFT, K_RIGHT, K_b, K_a };
		if(input->CheckSequence(konami, 10)) {
			hero_->Invulnerable(85000);
			AUDIO_MANAGER()->LoadMusic("musics/sf2Guile456.mid")->Play();
			konami_used_ = true;
		}
	}*/
}

Vector2D World::ActualOffset() {
    Vector2D result = -VIDEO_MANAGER()->video_size()*0.5;
    if(hero_) result += hero_->node()->modifier()->offset();
    return result;
}

bool World::VerifyPause() {
    ugdk::input::InputManager *input = Engine::reference()->input_manager();
    if(input->KeyPressed(ugdk::input::K_ESCAPE)) {
        MenuBuilder builder;
        Engine::reference()->PushScene(builder.BuildPauseMenu());
        return true;
    }
    return false;
}

bool IsNear(const TilePos &origin, const TilePos &pos, double radius) {
    if ((double)(abs((pos.i - origin.i)) + abs((pos.j - origin.j))) <= radius)
        return true;
    else if ((Tile::FromTilePos(pos) - Tile::FromTilePos(origin)).length() <= radius )
        return true;
    else return false;
}

void SpreadLight(GameMap &map, const TilePos &origin_pos, double radius) {

    list<Tile*>     queue;
    Vector2D        origin_world_pos = Tile::FromTilePos(origin_pos);
    Tile            *origin = Tile::GetFromMapPosition(map, origin_pos);
    VisionStrategy  vision;

    if (!origin) return;

    origin_world_pos.y = map.size() - origin_world_pos.y - 1;
    queue.push_back(origin);

    while (queue.size() > 0) {
        Tile *tile = *(queue.begin());
        queue.pop_front();
        if(tile == NULL) continue;
        if (!tile->checked() && IsNear(origin_pos, tile->pos(), radius)) {
            tile->Check();
            Vector2D tile_world_pos = Tile::FromTilePos(tile->pos());
            tile_world_pos.y = map.size() - tile_world_pos.y - 1;
            bool is_obstacle = (tile->object() == WALL) || (tile->object() == ENTRY),
                 is_visible = vision.IsLightVisible(origin_world_pos, tile_world_pos);
            if (is_obstacle || is_visible) {
                tile->set_visible(true);
                if (!is_obstacle)
                    for (int dir = Tile::BEGIN; dir < Tile::END; ++dir) {
                        Tile *next = tile->Next(map, (Tile::TileDir)dir);
                        if (next && !next->checked()) {
                            queue.push_back(next);
                        }
                    }
            }
        }
    }

}

void World::UpdateVisibility() {

    if(!hero_) return;

    GameMap& map = level_matrix();

    TilePos hero_pos = Tile::ToTilePos(hero_->world_position());

    hero_pos.i =  map.size() - hero_pos.i - 1;

    Tile::CleanVisibility(map);
    SpreadLight(map, hero_pos, 1.5*hero_->light_radius());

}

void World::Update(double delta_t) {

    if(VerifyPause()) return;

	content_node()->modifier()->set_visible(true);
    Scene::Update(delta_t);

#ifdef DEBUG
    VerifyCheats(delta_t);
#endif

    HandleCollisions();

    RemoveInactiveObjects();
    AddNewWorldObjects();
    
    world_node_->modifier()->set_offset(-ActualOffset());
	UpdateVisibility();

	if (!hero_)
        level_state_ = LevelManager::FINISH_DIE;

    if (level_state_ != LevelManager::NOT_FINISHED)
        LevelManager::reference()->FinishLevel(level_state_);

}

void World::End() {
    super::End();

    this->RemoveEntity(hud_);
	interface_node()->RemoveChild(hud_->node());
    delete hud_;
    hud_ = NULL;

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

void World::AddWorldObject(sprite::WorldObject* new_object, const ugdk::Vector2D& pos) {
    new_object->set_world_position(pos);
    new_world_objects_.push_front(new_object);
}

void World::AddNewWorldObjects() {
    for (list<sprite::WorldObject*>::iterator it = new_world_objects_.begin();
         it != new_world_objects_.end();
         ++it) {

        WorldObject *new_object = *it;
        world_objects_.push_front(new_object);
        this->AddEntity(new_object);

        world_node_->AddChild(new_object->node());

        if(new_object->collision_object() != NULL) {
            colliding_world_objects_.push_front(new_object);
            new_object->collision_object()->StartColliding();
        }
    }
    new_world_objects_.clear();
}

void World::AddHero(const ugdk::Vector2D& pos) {
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
            colliding_world_objects_.remove(*i);
            this->RemoveEntity(*i);
        }
    }
    world_objects_.remove_if(worldObjectIsDead);
}

void World::RemoveAll() {
    std::list<sprite::WorldObject*>::iterator i;
    for (i = world_objects_.begin(); i != world_objects_.end(); ++i) {
        if ( *i != hero_ ) {
            delete (*i);
        }
    }
    world_objects_.clear();
    if(hero_ != NULL) {
        this->world_node_->RemoveChild(hero_->node());
    }
    hero_ = NULL;
}

Vector2D World::FromScreenLinearCoordinates(const Vector2D& screen_coords) {
    Vector2D tx(sqrt(5.0)/4.0, -sqrt(5.0)/4.0);
    Vector2D ty(-sqrt(5.0)/2.0, -sqrt(5.0)/2.0);
    return (tx * screen_coords.x) + (ty * screen_coords.y);
}

Vector2D World::FromWorldLinearCoordinates(const Vector2D& world_coords) {
    Vector2D tx(54.0, -27.0);
    Vector2D ty(-54.0, -27.0);
    return (tx * world_coords.x) + (ty * world_coords.y);
}

Vector2D World::FromWorldCoordinates(const Vector2D& world_coords) {
    return FromWorldLinearCoordinates(world_coords);
}

Vector2D World::FromScreenCoordinates(const Vector2D& screen_coords) {
    Vector2D    global_screen_coords = screen_coords - WORLD()->world_node_->modifier()->offset(),
                transformed = FromScreenLinearCoordinates(global_screen_coords);
    return (transformed * (1.0/60.373835392));
}

const Vector2D World::ConvertLightRadius(double radius) {
    Vector2D ellipse_coords = Vector2D(2, 1) * radius * 60.373835392;
    return ellipse_coords;
}
    
sprite::WorldObject * World::hero_world_object() const {
    return dynamic_cast<WorldObject*> (hero_);
}

} // namespace scene
