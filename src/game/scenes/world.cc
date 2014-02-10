#include "world.h"

#include <cmath>
#include <iostream>
#include <algorithm>
#include <functional>
#include <ugdk/action/scene.h>
#include <ugdk/audio/music.h>
#include <ugdk/system/engine.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/textbox.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/time/module.h>
#include <ugdk/input/events.h>
#include <ugdk/input/module.h>
#include <ugdk/structure/intervalkdtree.h>
#include <ugdk/debug/profiler.h>

#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionmanager.h>

#include "game/config.h"
#include "game/components/caster.h"
#include "game/components/damageable.h"
#include "game/components/graphic.h"
#include "game/components/body.h"
#include "game/core/coordinates.h"
#include "game/map/room.h"
#include "game/sprites/worldobject.h"
#include "game/utils/hud.h"
#include "game/utils/levelmanager.h"
#include "game/renders/shape.h"
#include "game/renders/profiler.h"
#include "game/initializer.h"

namespace scene {

using namespace ugdk;
using namespace sprite;
using namespace utils;
using ugdk::structure::Box;
using std::bind;
using namespace std::placeholders;
using pyramidworks::collision::CollisionInstance;

namespace {
int render_sprites = 1; // 0 == nothing, 1 == sprites, 2 == lights
bool render_collision = false;
bool render_visibility = false;
bool render_profiler = false;
std::shared_ptr<graphic::TextBox> profiler_text(nullptr);
}

void VerifyCheats(const input::KeyPressedEvent& ev) {
    LevelManager *level_manager = LevelManager::reference();
    World* world = level_manager->current_level();
    sprite::WObjPtr hero = world->hero().lock();

    static uint32 last_level_warp = 0;
    if(ugdk::time::manager()->TimeSince(last_level_warp) > 100) {
        if (ev.keycode == ugdk::input::Keycode::p) {
            level_manager->SetNextLevel(level_manager->GetNextLevelID() + 1);
            world->FinishLevel(LevelManager::FINISH_WARP);
            last_level_warp = ugdk::time::manager()->TimeElapsed();

        } else if (ev.keycode == ugdk::input::Keycode::o) {
            unsigned int cur_level = level_manager->GetNextLevelID();
            if(cur_level > 0) {
                level_manager->SetNextLevel(cur_level - 1);
                world->FinishLevel(LevelManager::FINISH_WARP);
                last_level_warp = ugdk::time::manager()->TimeElapsed();
            }
        }
    }
    if(hero) {
        if(ev.keycode == input::Keycode::h) {
            if(ev.modifiers & input::Keymod::SHIFT)
            	hero->caster()->mana_blocks().Fill();
            hero->damageable()->life().Fill();
            hero->caster()->mana().Fill();
        }
        if(ev.keycode == input::Keycode::t)
            hero->set_world_position(core::FromScreenCoordinates(
            input::manager()->mouse().position()));
    }

    ugdk::graphic::Geometry& modifier = const_cast<ugdk::graphic::Geometry&>(world->camera());
    {
        math::Vector2D scale(1.0);
        if(ev.keycode == input::Keycode::NUMPAD_MULTIPLY)
            scale = scale * 1.4/1.0;
        if(ev.keycode == input::Keycode::NUMPAD_DIVIDE)
            scale = scale * 1.0/1.4;
        modifier *= graphic::Geometry(math::Vector2D(), scale);
    }

    if(ev.keycode == input::Keycode::l)
        ToggleLightsystem();
    
    if(ev.keycode == input::Keycode::k)
        ToggleShadowcasting();
    
    if(ev.keycode == input::Keycode::i)
        render_sprites = (render_sprites + 1) % 3;
    
    if(ev.keycode == input::Keycode::u)
        render_collision = !render_collision;
    
    if(ev.keycode == input::Keycode::j)
        render_visibility = !render_visibility;

    if(ev.scancode == input::Scancode::F9)
        render_profiler = !render_profiler;
    
    if (ev.scancode == input::Scancode::F10) {
        const auto& datalist = ugdk::system::profile_data_list();
        if (!datalist.empty()) {
            std::stringstream msg;
            renders::SectionDataToString(msg, "", datalist.back());
            profiler_text->ChangeMessage(msg.str());
        }
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
}

bool FinishLevelTask(double dt, const LevelManager::LevelState* state) {
    if (*state != LevelManager::NOT_FINISHED) {
        LevelManager::reference()->FinishLevel(*state);
        return false;
    }
    return true;
}

bool RoomCompareByPositionAndPointer(map::Room* a, map::Room* b) {
    Vector2D  ap = core::FromWorldCoordinates(a->position()+a->size()/2.0),
              bp = core::FromWorldCoordinates(b->position()+b->size()/2.0);
    if(ap.y < bp.y)
        return true;
    if(ap.y == bp.y) {
        if(ap.x < bp.x)
            return true;
        if(ap.x == bp.x)
            return a < b;
    }
    return false;
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
    hud_(nullptr)
{

    set_identifier("World");

    hud_ = new utils::Hud(this);
    this->AddEntity(hud_);
    this->AddTask(bind(&World::updateRooms, this, _1));
    this->AddTask(ugdk::system::Task(bind(FinishLevelTask, _1, &level_state_), 1.0));
    this->AddTask(ugdk::system::Task([this](double) {
        Vector2D result = ugdk::graphic::manager()->canvas()->size()*0.5;
        if(hero_)
            result -= core::FromWorldCoordinates(hero_->world_position()) 
                                * camera_.CalculateScale().x;
        camera_.set_offset(Vector2D(std::floor(result.x), std::floor(result.y)));
    }, 1.0));

    this->AddTask(ugdk::system::Task([this](double) {
        while(!queued_moves_.empty()) {
            auto p = this->queued_moves_.front();
            this->queued_moves_.pop();

            if (auto wobj = p.first.lock()) {
                wobj->current_room()->RemoveObject(wobj);
                p.second->ForceAddObject(wobj);

                if (wobj == this->hero_)
                    ChangeFocusedRoom(p.second);
            } 
        }
    }, 0.6));

#ifdef HORUSEYE_DEBUG_TOOLS
    this->event_handler().AddListener<input::KeyPressedEvent>(VerifyCheats);
#endif

    if (!profiler_text)
        profiler_text.reset(new graphic::TextBox(
            "Press F10 to fetch profiler data.",
            graphic::manager()->canvas()->size().x,
            TEXT_MANAGER()->current_font()));

    set_render_function([this](graphic::Canvas& canvas) {
        ugdk::graphic::manager()->shaders().ChangeFlag(ugdk::graphic::Manager::Shaders::USE_LIGHT_BUFFER, true);
        canvas.PushAndCompose(this->camera_);
        if(render_sprites == 1)
            for(const map::Room* room : active_rooms_)
                room->Render(canvas);
        else if(render_sprites == 2)
            DrawTexture(ugdk::graphic::manager()->light_buffer(), canvas);

        ugdk::graphic::manager()->shaders().ChangeFlag(ugdk::graphic::Manager::Shaders::USE_LIGHT_BUFFER, false);
        if(render_collision)
            for(auto collobject : collision_manager_.active_objects())
                renders::DrawCollisionObject(collobject, canvas);
        if(render_visibility)
            for(auto collobject : visibility_manager_.active_objects())
                renders::DrawCollisionObject(collobject, canvas);
        
        canvas.PopGeometry();
        {
            ugdk::debug::ProfileSection section("Hud");
            this->hud_->node()->Render(canvas);
        }

        if (render_profiler)
            profiler_text->Draw(canvas);
    });

    SetupCollisionManager();
}

// Destrutor
World::~World() {
}

void World::Start() {
    ChangeFocusedRoom(hero_initial_room_);
    if(hero_)
        if(map::Room* initial_room = findRoom(hero_initial_room_))
            initial_room->AddObject(hero_, hero_initial_position_, map::POSITION_ABSOLUTE);
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

void World::SetHero(const sprite::WObjPtr& hero) {
    hero_ = hero;
}
    
void World::QueueRoomChange(const sprite::WObjWeakPtr& wobj, map::Room* next_room) {
    queued_moves_.emplace(wobj, next_room);
}

void World::SetupCollisionManager() {
    collision_manager_.ChangeClassParent("Hero", "Creature");
    collision_manager_.ChangeClassParent("Mummy", "Creature");

    collision_manager_.ChangeClassParent("Block", "Wall");
    collision_manager_.ChangeClassParent("Door", "Wall");

    this->AddTask(collision_manager_.GenerateHandleCollisionTask(0.75));
}
    
void World::RenderLight(ugdk::graphic::Canvas& canvas) const {
    ugdk::debug::ProfileSection section("World::RenderLight");
    canvas.PushAndCompose(camera_);
    for(const map::Room* room : active_rooms_)
        room->RenderLight(canvas);
    canvas.PopGeometry();
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

void World::ChangeFocusedRoom(const std::string& name) {
    ChangeFocusedRoom(findRoom(name));
}

void World::ChangeFocusedRoom(map::Room* room) {
    if(!room) return;
    for(map::Room* active_room : active_rooms_)
        active_room->Deactivate();
    active_rooms_.clear();
    active_rooms_.reserve(1 + room->neighborhood().size());
    active_rooms_.push_back(room);
    for(const std::string& neightbor_name : room->neighborhood())
        if(map::Room* neightbor = findRoom(neightbor_name))
            active_rooms_.push_back(neightbor);
    for(map::Room* active_room : active_rooms_)
        active_room->Activate();
    std::sort(active_rooms_.begin(), active_rooms_.end(), RoomCompareByPositionAndPointer);
}

bool World::IsRoomActive(const std::string& name) const {
    return IsRoomActive(findRoom(name));
}

bool World::IsRoomActive(const map::Room* room) const {
    return std::find(active_rooms_.begin(), active_rooms_.end(), room) != active_rooms_.end();
}
    
map::Room* World::FindRoomFromPoint(const math::Vector2D& point) const {
    std::list<map::Room*> results;
    rooms_by_location_.FindIntersectingItems(Box<2>(point, point), std::back_inserter(results));
    assert(results.size() <= 1);
    return results.empty() ? nullptr : results.front();
}
    
map::Room* World::findRoom(const std::string& name) const {
    auto it = rooms_.find(name);
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
