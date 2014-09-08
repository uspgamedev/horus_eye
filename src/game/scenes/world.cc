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
#include <ugdk/graphic/drawable/functions.h>
#include <ugdk/graphic/text/textbox.h>
#include <ugdk/graphic/text/textmanager.h>
#include <ugdk/time/module.h>
#include <ugdk/input/events.h>
#include <ugdk/input/module.h>
#include <ugdk/structure/intervalkdtree.h>
#include <ugdk/debug/profiler.h>

#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionmanager.h>

#include "game/config.h"
#include "game/campaigns/campaign.h"
#include "game/components/caster.h"
#include "game/components/damageable.h"
#include "game/components/graphic.h"
#include "game/components/body.h"
#include "game/core/coordinates.h"
#include "game/map/room.h"
#include "game/scenes/console.h"
#include "game/scenes/lightrendering.h"
#include "game/sprites/worldobject.h"
#include "game/utils/hud.h"
#include "game/renders/shape.h"
#include "game/renders/profiler.h"
#include "game/initializer.h"
#include "game/builders/goodmenubuilder.h"

namespace scene {

using namespace ugdk;
using namespace sprite;
using namespace utils;
using ugdk::structure::Box;
using std::bind;
using namespace std::placeholders;
using pyramidworks::collision::CollisionInstance;

namespace {
bool render_sprites = true;
bool render_collision = false;
bool render_visibility = false;
bool render_profiler = false;
std::shared_ptr<graphic::TextBox> profiler_text(nullptr);
}

void VerifyCheats(World* world, const input::KeyPressedEvent& ev) {
    sprite::WObjPtr hero = world->hero().lock();

    static uint32 last_level_warp = 0;
    /*
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
    */
    if(hero) {
        if(ev.keycode == input::Keycode::h) {
            if(ev.modifiers & input::Keymod::SHIFT)
            	hero->caster()->mana_blocks().Fill();
            hero->damageable()->life().Fill();
            hero->caster()->mana().Fill();
        }
        if(ev.keycode == input::Keycode::t)
            hero->set_world_position(
                core::FromScreenCoordinates(
                    world, 
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
    
    if (ev.keycode == input::Keycode::m)
        hero->damageable()->TakeDamage(1000.0);

    if(ev.keycode == input::Keycode::l)
        world->light_rendering()->ToggleLightsystem();
    
    if(ev.keycode == input::Keycode::k)
        world->light_rendering()->ToggleShadowcasting();
    
    if(ev.keycode == input::Keycode::i)
        render_sprites = !render_sprites;
    
    if(ev.keycode == input::Keycode::u)
        render_collision = !render_collision;
    
    if(ev.keycode == input::Keycode::j)
        render_visibility = !render_visibility;

    if(ev.scancode == input::Scancode::F9)
        render_profiler = !render_profiler;
    
    if (ev.scancode == input::Scancode::F10 || ev.scancode == input::Scancode::MENU) {
        const auto& datalist = ugdk::system::profile_data_list();
        if (!datalist.empty()) {
            std::stringstream msg;
            renders::SectionDataToString(msg, "", datalist.back());
            profiler_text->ChangeMessage(msg.str());
        }
        if (ev.scancode == input::Scancode::MENU)
            render_profiler = true;
    }

    if (ev.scancode == input::Scancode::GRAVE) {
        ugdk::system::PushScene(new Console);
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
            
World::World(const ugdk::math::Integer2D& size, const ugdk::script::VirtualObj& vobj) 
  :   
    // World Layout
    size_(size),
    rooms_by_location_(Box<2>(Vector2D(-1.0, -1.0), Vector2D(size)), 4),

    // Game logic
    campaign_(nullptr),
    collision_manager_(Box<2>(Vector2D(-1.0, -1.0), Vector2D(size))),
    visibility_manager_(Box<2>(Vector2D(-1.0, -1.0), Vector2D(size))),
    vobj_(vobj),

    // Graphic
    hud_(nullptr)
{

    set_identifier("World");

    hud_ = new utils::Hud(this);
    this->AddEntity(hud_);
    this->AddTask(bind(&World::updateRooms, this, _1));
    this->AddTask(ugdk::system::Task([this](double) {
        Vector2D result = ugdk::graphic::manager()->screen()->size()*0.5;
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
    
    this->event_handler().AddListener<input::KeyPressedEvent>([](const input::KeyPressedEvent& key) {
        if (key.scancode == ugdk::input::Scancode::ESCAPE)
            ugdk::system::PushScene(builder::PauseMenu);
    });

#ifdef HORUSEYE_DEBUG_TOOLS
    this->event_handler().AddListener<input::KeyPressedEvent>(bind(VerifyCheats, this, _1));
#endif

    if (!profiler_text)
        profiler_text.reset(new graphic::TextBox(
            "Press F10 to fetch profiler data.",
            graphic::manager()->screen()->size().x,
            TEXT_MANAGER()->current_font()));

    set_render_function([this](graphic::Canvas& canvas) {

        auto& shaders = ugdk::graphic::manager()->shaders();
        shaders.ChangeFlag(ugdk::graphic::Manager::Shaders::USE_LIGHT_BUFFER, true);
        canvas.ChangeShaderProgram(shaders.current_shader());
        canvas.PushAndCompose(this->camera_);
        if(render_sprites)
            for(const map::Room* room : active_rooms_)
                room->Render(canvas);

        shaders.ChangeFlag(ugdk::graphic::Manager::Shaders::USE_LIGHT_BUFFER, false);
        canvas.ChangeShaderProgram(shaders.current_shader());

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
    RemoveAllEntities();
    removeAllRooms();
}

void World::Start(campaigns::Campaign* campaign) {
    campaign_ = campaign;
    (vobj_ | "Start")(this, campaign->implementation());
}

void World::End() {
    super::End();
    campaign_->InformLevelFinished();
    (vobj_ | "End")(this, campaign_->implementation());
}

ugdk::action::Scene* World::CreateLightRenderingScene() {
    return nullptr;
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
    for(const map::Room* room : active_rooms_)
        room->RenderLight(canvas);
}

void World::AddRoom(map::Room* room) {
    if(room && !room->name().empty()) {
        room->DefineLevel(this);
        room->Sort();
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
