#include "world.h"

#include <cmath>
#include <iostream>
#include <algorithm>
#include <ugdk/portable/tr1.h>
#include FROM_TR1(functional)
#include <ugdk/action/scene.h>
#include <ugdk/action/generictask.h>
#include <ugdk/audio/music.h>
#include <ugdk/base/engine.h>
#include <externals/ugdk-videomanager.h>
#include <ugdk/time/timemanager.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/util/intervalkdtree.h>

#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionmanager.h>

#include "game/map/room.h"

#include "game/scenes/menu.h"
#include "game/sprites/worldobject.h"
#include "game/components/caster.h"
#include "game/components/damageable.h"
#include "game/components/shape.h"
#include "game/utils/hud.h"
#include "game/utils/levelmanager.h"

namespace scene {

using namespace ugdk;
using namespace sprite;
using namespace utils;
using ugdk::action::GenericTask;
using std::tr1::bind;
using namespace std::tr1::placeholders;
using pyramidworks::collision::CollisionInstance;

bool VerifyCheats(double delta_t) {
    ugdk::input::InputManager *input = Engine::reference()->input_manager();
    LevelManager *level_manager = LevelManager::reference();
    World* world = level_manager->get_current_level();
    WorldObject* hero = world->hero();

    static uint32 last_level_warp = 0;
    if(Engine::reference()->time_handler()->TimeSince(last_level_warp) > 1250) {
        if (input->KeyPressed(ugdk::input::K_p)) {
            level_manager->SetNextLevel(level_manager->GetNextLevelID() + 1);
            world->FinishLevel(LevelManager::FINISH_WARP);
            last_level_warp = Engine::reference()->time_handler()->TimeElapsed();

        } else if (input->KeyPressed(ugdk::input::K_o)) {
            unsigned int cur_level = level_manager->GetNextLevelID();
            if(cur_level > 0) {
                level_manager->SetNextLevel(cur_level - 1);
                world->FinishLevel(LevelManager::FINISH_WARP);
                last_level_warp = Engine::reference()->time_handler()->TimeElapsed();
            }
        }
    }
    if(hero) {
        if(input->KeyPressed(ugdk::input::K_h)) {
            if(input->KeyDown(ugdk::input::K_LSHIFT))
            	hero->caster()->mana_blocks().Fill();
            hero->damageable()->life().Fill();
            hero->caster()->mana().Fill();
        }
        if(input->KeyPressed(ugdk::input::K_t))
            hero->set_world_position(World::FromScreenCoordinates(input->GetMousePosition()));
    }

    if(input->KeyPressed(ugdk::input::K_l)) {
        static bool lights_on = true;
        VIDEO_MANAGER()->SetLightSystem(lights_on = !lights_on);
    }

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

    return true;
}

bool UpdateOffset(double dt) {
    World* world = WORLD();
    Vector2D result = VIDEO_MANAGER()->video_size()*0.5;
    if(world->hero()) result -= world->hero()->node()->modifier()->offset();
    world->content_node()->modifier()->set_offset(result);
    return true;
}

bool FinishLevelTask(double dt, const LevelManager::LevelState* state) {
    if (*state != LevelManager::NOT_FINISHED) {
        LevelManager::reference()->FinishLevel(*state);
        return false;
    }
    return true;
}

World::World() 
    :   Scene(),
        hero_(NULL),
        size_(10, 10),
        level_state_(LevelManager::NOT_FINISHED),
        collision_manager_(NULL),
        visibility_manager_(NULL) {

    content_node()->modifier()->ToggleFlag(ugdk::graphic::Modifier::TRUNCATES_WHEN_APPLIED);

    layers_[BACKGROUND_LAYER] = new graphic::Node;
    layers_[FOREGROUND_LAYER] = new graphic::Node;
    layers_[BACKGROUND_LAYER]->set_zindex(BACKGROUND_LAYER);
    layers_[FOREGROUND_LAYER]->set_zindex(FOREGROUND_LAYER);

    content_node()->AddChild(layers_[BACKGROUND_LAYER]);
    content_node()->AddChild(layers_[FOREGROUND_LAYER]);

    hud_ = new utils::Hud(this);
    interface_node()->AddChild(hud_->node());
    this->AddEntity(hud_);

    this->AddTask(new GenericTask(bind(&World::updateRooms, this, _1)));

    this->AddTask(new GenericTask(bind(FinishLevelTask, _1, &level_state_), 1000));
//#ifdef DEBUG
    this->AddTask(new GenericTask(VerifyCheats));
//#endif
    this->AddTask(new GenericTask(UpdateOffset));
}

// Destrutor
World::~World() {
    if(collision_manager_) delete collision_manager_;
}

void World::Start() {
    map::Room* room = hero_initial_room_.empty() ? NULL : rooms_[hero_initial_room_];
    if(room) {
        ActivateRoom(hero_initial_room_);
        if(hero_)
            room->AddObject(hero_, hero_initial_position_, map::POSITION_ABSOLUTE);
    }
}

void World::End() {
    super::End();

    this->RemoveEntity(hud_);
    interface_node()->RemoveChild(hud_->node());
    delete hud_;
    hud_ = NULL;

    if(hero_ != NULL) {
        //hero_->Invulnerable(0);
        hero_->shape()->Deactivate();
        this->RemoveEntity(hero_);
        layer_node(hero_->layer())->RemoveChild(hero_->node());
        hero_ = NULL;
    }

    RemoveAllEntities();
    removeAllRooms();
}

void World::SetHero(sprite::WorldObject *hero) {
    hero_ = hero;
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
    Vector2D    global_screen_coords = screen_coords - WORLD()->content_node()->modifier()->offset(),
                transformed = FromScreenLinearCoordinates(global_screen_coords);
    return (transformed * (1.0/60.373835392));
}

const Vector2D World::ConvertLightRadius(double radius) {
    Vector2D ellipse_coords = Vector2D(2, 1) * radius * 60.373835392;
    return ellipse_coords;
}
    
void World::SetupCollisionManager() {
    ugdk::Vector2D min_coords( -1.0, -1.0 ), max_coords(size_);
    ugdk::ikdtree::Box<2> box(min_coords.val, max_coords.val);
    collision_manager_ = new pyramidworks::collision::CollisionManager(box);

    collision_manager_->Generate("WorldObject");

    collision_manager_->Generate("Creature", "WorldObject");
    collision_manager_->Generate("Hero", "Creature");
    collision_manager_->Generate("Mummy", "Creature");

    collision_manager_->Generate("Wall", "WorldObject");
    collision_manager_->Generate("Block", "Wall");
    collision_manager_->Generate("Door", "Wall");

    collision_manager_->Generate("Item", "WorldObject");
    collision_manager_->Generate("Projectile", "WorldObject");
    collision_manager_->Generate("Button", "WorldObject");
    collision_manager_->Generate("Explosion", "WorldObject");

    this->AddTask(collision_manager_->GenerateHandleCollisionTask());
    
    visibility_manager_ = new pyramidworks::collision::CollisionManager(box);
    visibility_manager_->Generate("Opaque");
}

void World::AddRoom(map::Room* room) {
    if(room && !room->name().empty()) {
        room->DefineLevel(this);
        rooms_[room->name()] = room;
    }
}

void World::ActivateRoom(const std::string& name) {
    map::Room* room = rooms_[name];
    if(room && !IsRoomActive(room)) {
        active_rooms_.push_back(room);
        layer_node(BACKGROUND_LAYER)->AddChild(room->floor());
        for(map::Room::WObjListConstIterator it = room->begin(); it != room->end(); ++it)
            layer_node((*it)->layer())->AddChild((*it)->node());
    }
}

void World::DeactivateRoom(const std::string& name) {
    map::Room* room = findRoom(name);
    if(room && IsRoomActive(room)) {
        active_rooms_.remove(room);
        layer_node(BACKGROUND_LAYER)->RemoveChild(room->floor());
        for(map::Room::WObjListConstIterator it = room->begin(); it != room->end(); ++it)
            layer_node((*it)->layer())->RemoveChild((*it)->node());
    }
}

bool World::IsRoomActive(const std::string& name) const {
    return IsRoomActive(findRoom(name));
}

bool World::IsRoomActive(const map::Room* room) const {
    return std::find(active_rooms_.begin(), active_rooms_.end(), room) != active_rooms_.end();
}

map::Room* World::findRoom(const std::string& name) const {
    std::tr1::unordered_map<std::string, map::Room*>::const_iterator it = rooms_.find(name);
    if(it == rooms_.end()) return NULL;
    return it->second;
}

bool World::updateRooms(double dt) {
    for(std::list<map::Room*>::const_iterator it = active_rooms_.begin(); it != active_rooms_.end(); ++it)
        (*it)->Update(dt);
    return true;
}

void World::removeAllRooms() {
    for(std::tr1::unordered_map<std::string, map::Room*>::iterator it = rooms_.begin(); it != rooms_.end(); ++it)
        delete it->second;
    rooms_.clear();
    active_rooms_.clear();
}

} // namespace scene
