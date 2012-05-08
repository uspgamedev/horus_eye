#include <cmath>
#include <iostream>
#include <functional>
#include <ugdk/action/scene.h>
#include <ugdk/action/generictask.h>
#include <ugdk/audio/music.h>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/time/timemanager.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/util/intervalkdtree.h>

#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionmanager.h>

#include "world.h"

#include "game/scenes/menu.h"
#include "game/scenes/menubuilder.h"

#include "game/sprites/worldobject.h"
#include "game/components/hero.h"
#include "game/components/damageable.h"
#include "game/utils/tile.h"
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
using component::Hero;
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
            component::Hero* hero_logic = static_cast<component::Hero*>(hero->logic());
            if(input->KeyDown(ugdk::input::K_LSHIFT))
                hero_logic->mana_blocks().Fill();
            hero->damageable()->life().Fill();
            hero_logic->mana().Fill();
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

World::World(sprite::WorldObject *hero, utils::ImageFactory *factory) 
    :   Scene(),
        hero_(hero),
        remaining_enemies_(0),
        max_enemies_(0),
        image_factory_(factory),
        level_state_(LevelManager::NOT_FINISHED),
        konami_used_(false),
        lights_on_(true),
        num_button_not_pressed_(0),
        collision_manager_(NULL) {

    content_node()->modifier()->ToggleFlag(ugdk::graphic::Modifier::TRUNCATES_WHEN_APPLIED);

    hud_ = new utils::Hud(this);
    interface_node()->AddChild(hud_->node());
    this->AddEntity(hud_);

    this->AddTask(new ugdk::action::GenericTask(std::tr1::bind(FinishLevelTask, std::tr1::placeholders::_1, &level_state_), 1000));
#ifdef DEBUG
    this->AddTask(new ugdk::action::GenericTask(VerifyCheats));
#endif
    this->AddTask(new ugdk::action::GenericTask(UpdateOffset));
}

// Destrutor
World::~World() {
    if(image_factory_) delete image_factory_;
    if(collision_manager_) delete collision_manager_;
}

void World::End() {
    super::End();

    this->RemoveEntity(hud_);
    interface_node()->RemoveChild(hud_->node());
    delete hud_;
    hud_ = NULL;

    if(hero_ != NULL) {
        //hero_->Invulnerable(0);
        hero_->collision_object()->StopColliding();
        this->RemoveEntity(hero_);
        content_node()->RemoveChild(hero_->node());
        hero_ = NULL;
    }

    this->RemoveAllEntities();
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
    QueuedAddEntity(new_object);
}

void World::AddHero(const ugdk::Vector2D& pos) {
    this->AddWorldObject(hero_, pos);
}

int World::CountRemainingEnemies() {
    return remaining_enemies_;
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
    
sprite::WorldObject * World::hero_world_object() const {
    return hero_;
}

void World::SetupCollisionManager() {
    double min_coords[2] = { -1, -1 };
    double max_coords[2] = { this->level_width(), this->level_height() };
    ugdk::ikdtree::Box<2> box(min_coords, max_coords);
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
}

} // namespace scene
