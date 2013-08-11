#include "world.h"

#include <cmath>
#include <iostream>
#include <algorithm>
#include <functional>
#include <ugdk/action/scene.h>
#include <ugdk/audio/music.h>
#include <ugdk/system/engine.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/node.h>
#include <ugdk/time/module.h>
#include <ugdk/input/module.h>
#include <ugdk/structure/intervalkdtree.h>

#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionmanager.h>

#include "game/map/room.h"

#include "game/sprites/worldobject.h"
#include "game/core/coordinates.h"
#include "game/components/caster.h"
#include "game/components/damageable.h"
#include "game/components/graphic.h"
#include "game/components/shape.h"
#include "game/utils/hud.h"
#include "game/utils/levelmanager.h"
#include "game/initializer.h"

namespace scene {

using namespace ugdk;
using namespace sprite;
using namespace utils;
using ugdk::structure::Box;
using std::bind;
using namespace std::placeholders;
using pyramidworks::collision::CollisionInstance;

bool VerifyCheats(double dt) {
    ugdk::input::Manager *input = ugdk::input::manager();
    LevelManager *level_manager = LevelManager::reference();
    World* world = level_manager->current_level();
    WorldObject* hero = world->hero();

    static uint32 last_level_warp = 0;
    if(ugdk::time::manager()->TimeSince(last_level_warp) > 1250) {
        if (input->KeyPressed(ugdk::input::K_p)) {
            level_manager->SetNextLevel(level_manager->GetNextLevelID() + 1);
            world->FinishLevel(LevelManager::FINISH_WARP);
            last_level_warp = ugdk::time::manager()->TimeElapsed();

        } else if (input->KeyPressed(ugdk::input::K_o)) {
            unsigned int cur_level = level_manager->GetNextLevelID();
            if(cur_level > 0) {
                level_manager->SetNextLevel(cur_level - 1);
                world->FinishLevel(LevelManager::FINISH_WARP);
                last_level_warp = ugdk::time::manager()->TimeElapsed();
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
            hero->set_world_position(core::FromScreenCoordinates(input->GetMousePosition()));
    }

    ugdk::graphic::Geometry& modifier = world->content_node()->geometry();
    {
        math::Vector2D scale(1.0);
        if(input->KeyPressed(ugdk::input::K_KP_MULTIPLY))
            scale = scale * 1.4/1.0;
        if(input->KeyPressed(ugdk::input::K_KP_DIVIDE))
            scale = scale * 1.0/1.4;
        modifier *= graphic::Geometry(math::Vector2D(), scale);
    }

    // EASTER EGG/TODO: remove before any release!
    // Also erase musics/sf2Guile456.mid
    /*if(!konami_used_) {
        Key konami[10] = { K_UP, K_UP, K_DOWN, K_DOWN, K_LEFT, K_RIGHT, K_LEFT, K_RIGHT, K_b, K_a };
        if(input->CheckSequence(konami, 10)) {
            hero_->Invulnerable(85000);
            ugdk::audio::manager()->LoadMusic("musics/sf2Guile456.mid")->Play();
            konami_used_ = true;
        }
    }*/

    return true;
}

bool UpdateOffset(double dt) {
    World* world = WORLD();
    auto mgr = ugdk::graphic::manager();
    Vector2D result = mgr->video_size()*0.5;
    if(world->hero()) result -= core::FromWorldCoordinates(world->hero()->world_position()) * world->content_node()->geometry().CalculateScale().x;
    world->content_node()->geometry().set_offset(Vector2D(std::floor(result.x), std::floor(result.y)));
    return true;
}

bool FinishLevelTask(double dt, const LevelManager::LevelState* state) {
    if (*state != LevelManager::NOT_FINISHED) {
        LevelManager::reference()->FinishLevel(*state);
        return false;
    }
    return true;
}

World::World(const ugdk::math::Integer2D& size) 
  :   
    // World Layout
    size_(size),
    rooms_by_location_(Box<2>(Vector2D(-1.0, -1.0), Vector2D(size)), 4),

    // Game logic
    level_state_(LevelManager::NOT_FINISHED),
    collision_manager_(Box<2>(Vector2D(-1.0, -1.0), Vector2D(size))),
    visibility_manager_(Box<2>(Vector2D(-1.0, -1.0), Vector2D(size))),

    // Graphic
    hud_(nullptr),
    content_node_(new graphic::Node) ,

    // Hero
    hero_(nullptr)
{
    //content_node()->geometry().ToggleFlag(ugdk::graphic::Modifier::TRUNCATES_WHEN_APPLIED);
    layers_[BACKGROUND_LAYER] = new graphic::Node;
    layers_[FOREGROUND_LAYER] = new graphic::Node;
    layers_[BACKGROUND_LAYER]->set_zindex(BACKGROUND_LAYER);
    layers_[FOREGROUND_LAYER]->set_zindex(FOREGROUND_LAYER);

    content_node()->AddChild(layers_[BACKGROUND_LAYER]);
    content_node()->AddChild(layers_[FOREGROUND_LAYER]);

    hud_ = new utils::Hud(this);
    this->AddEntity(hud_);

    this->AddTask(bind(&World::updateRooms, this, _1));

    this->AddTask(bind(FinishLevelTask, _1, &level_state_), 1000);
//#ifdef DEBUG
    this->AddTask(VerifyCheats);
//#endif
    this->AddTask(UpdateOffset, 1.0);

    set_render_function([this](const graphic::Geometry& geometry, const graphic::VisualEffect& effect) {
        ugdk::graphic::manager()->shaders().ChangeFlag(ugdk::graphic::Manager::Shaders::USE_LIGHT_BUFFER, true);
        content_node()->Render(geometry, effect);
        
        ugdk::graphic::manager()->shaders().ChangeFlag(ugdk::graphic::Manager::Shaders::USE_LIGHT_BUFFER, false);
        this->hud_->node()->Render(geometry, effect);
    });

    SetupCollisionManager();
}

// Destrutor
World::~World() {
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
    delete hud_;
    hud_ = NULL;

    RemoveAllEntities();
    removeAllRooms();
}

void World::Focus() {
    Scene::Focus();
    this->set_active(true);
}

void World::DeFocus() {
    Scene::DeFocus();
    this->set_active(false);
}

void World::SetHero(sprite::WorldObject *hero) {
    hero_ = hero;
}

void World::SetupCollisionManager() {
    collision_manager_.Generate("Creature");
    collision_manager_.Generate("Hero", "Creature");
    collision_manager_.Generate("Mummy", "Creature");

    collision_manager_.Generate("Wall");
    collision_manager_.Generate("Block", "Wall");
    collision_manager_.Generate("Door", "Wall");

    collision_manager_.Generate("Item");
    collision_manager_.Generate("Projectile");
    collision_manager_.Generate("Button");
    collision_manager_.Generate("Explosion");

    this->AddTask(collision_manager_.GenerateHandleCollisionTask());
    
    visibility_manager_.Generate("Opaque");
}

void World::AddRoom(map::Room* room) {
    if(room && !room->name().empty()) {
        room->DefineLevel(this);
        rooms_[room->name()] = room;
        rooms_by_location_.Insert(
            Box<2>(Vector2D(room->position()), Vector2D(room->position() + room->size())),
            room);
    }
}

void World::ActivateRoom(const std::string& name) {
    map::Room* room = rooms_[name];
    if(room && !IsRoomActive(room)) {
        active_rooms_.push_back(room);
        layer_node(BACKGROUND_LAYER)->AddChild(room->floor());
        for(map::Room::WObjListConstIterator it = room->begin(); it != room->end(); ++it)
            if((*it)->graphic())
                (*it)->graphic()->InsertIntoLayers(layers());
    }
}

void World::DeactivateRoom(const std::string& name) {
    map::Room* room = findRoom(name);
    if(room && IsRoomActive(room)) {
        active_rooms_.remove(room);
        layer_node(BACKGROUND_LAYER)->RemoveChild(room->floor());
        for(map::Room::WObjListConstIterator it = room->begin(); it != room->end(); ++it)
            if((*it)->graphic())
                (*it)->graphic()->RemoveFromLayers(layers());
    }
}

bool World::IsRoomActive(const std::string& name) const {
    return IsRoomActive(findRoom(name));
}

bool World::IsRoomActive(const map::Room* room) const {
    return std::find(active_rooms_.begin(), active_rooms_.end(), room) != active_rooms_.end();
}

map::Room* World::findRoom(const std::string& name) const {
    std::unordered_map<std::string, map::Room*>::const_iterator it = rooms_.find(name);
    if(it == rooms_.end()) return nullptr;
    return it->second;
}

bool World::updateRooms(double dt) {
    for(map::Room* room : active_rooms_)
        room->Update(dt);
    return true;
}

void World::removeAllRooms() {
    rooms_by_location_.Clear();
    for(const auto& it : rooms_)
        delete it.second;
    rooms_.clear();
    active_rooms_.clear();
}

} // namespace scene
