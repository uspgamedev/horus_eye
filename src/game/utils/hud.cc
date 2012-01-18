#include <ugdk/math/vector2D.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/drawable/text.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/base/engine.h>
#include <ugdk/action/animation.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/modifier.h>
#include <ugdk/graphic/textmanager.h>

#include "hud.h"

#include "game/scenes/world.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/constants.h"
#include "game/skills/combatart.h"
#include "game/sprites/creatures/hero.h"
#include "game/skills/skill.h"

#define LIFE_IMAGE_WIDTH Constants::LIFE_IMAGE_WIDTH
#define LIFE_IMAGE_HEIGHT Constants::LIFE_IMAGE_HEIGHT

#define LIFE_BAR_OFFSET_X Constants::LIFE_BAR_OFFSET_X
#define LIFE_BAR_OFFSET_Y Constants::LIFE_BAR_OFFSET_Y
#define LIFE_BAR_WIDTH Constants::LIFE_BAR_WIDTH
#define LIFE_BAR_HEIGHT Constants::LIFE_BAR_HEIGHT

#define MANA_BAR_OFFSET_X Constants::MANA_BAR_OFFSET_X
#define MANA_BAR_OFFSET_Y Constants::MANA_BAR_OFFSET_Y
#define MANA_BAR_WIDTH Constants::MANA_BAR_WIDTH
#define MANA_BAR_HEIGHT Constants::MANA_BAR_HEIGHT

#define TOTEM_WIDTH Constants::TOTEM_WIDTH
#define TOTEM_HEIGHT Constants::TOTEM_HEIGHT
#define TOTEM_OFFSET_X Constants::TOTEM_OFFSET_X
#define TOTEM_OFFSET_Y Constants::TOTEM_OFFSET_Y

#define ENEMY_COUNTER_OFFSET_X -10
#define ENEMY_COUNTER_OFFSET_Y  30

#define FPS_BAR_OFFSET_X  10
#define FPS_BAR_OFFSET_Y  30

#define VIDEO_Y VIDEO_MANAGER()->video_size().y 
#define VIDEO_X VIDEO_MANAGER()->video_size().x 

#define NUMBER_WIDTH 18
#define NUMBER_HEIGHT 16

using namespace ugdk;
using namespace scene;

namespace utils {

static Node* CreateBarOn(Node* container, TexturedRectangle* bar_image, HudImageFactory& img_fac) {

    TexturedRectangle 
        *totem_bar_image = img_fac.TotemImage(),
        *totem_bar_bottom = img_fac.TotemBottomImage();

    Node* totem_bottom_node = new Node(totem_bar_bottom);
    totem_bottom_node->modifier()->set_offset(Vector2D(0.0f, static_cast<float>(-totem_bar_bottom->height())));
    totem_bottom_node->set_zindex(1.0f);
    container->AddChild(totem_bottom_node);

    Node* totem_top_node = new Node(totem_bar_image);
    totem_top_node->modifier()->set_offset(Vector2D(0.0f, static_cast<float>(-totem_bar_bottom->height() - totem_bar_image->height())));
    totem_top_node->set_zindex(-1.0f);
    container->AddChild(totem_top_node);

    Node* bar_node = new Node;
    bar_node->modifier()->set_offset(Vector2D(totem_bar_image->width() * 0.5f - bar_image->width() * 0.5f, totem_bar_bottom->height() * (-1.0f)));
    bar_node->set_zindex(-0.5f);
    container->AddChild(bar_node);

    return bar_node;
}

// Aviso: maximo de 999 mumias no display de inimigos restantes!
Hud::Hud(World* world) : node_(new Node), displayed_skill_(NULL) {
    /*Image* number = VIDEO_MANAGER()->LoadImage("data/images/numbers2.png");
    number->set_frame_size(Vector2D(NUMBER_WIDTH, NUMBER_HEIGHT));*/

    // Criando sprites da life bar
    HudImageFactory img_fac;

    TexturedRectangle* back_image = img_fac.BackImage();
    TexturedRectangle* eye_image = img_fac.EyeImage();

    Node* eye_node = new Node(eye_image);
    eye_node->modifier()->set_offset(Vector2D(VIDEO_X/2, VIDEO_Y) - Vector2D(eye_image->width() * 0.5f, eye_image->height()));
    eye_node->set_zindex(0.5f);

    Node* back_right_node = new Node(back_image);
    back_right_node->modifier()->set_offset(Vector2D(VIDEO_X/2 + eye_image->width() * 0.5f, VIDEO_Y - back_image->height()));

    Node* back_left_node = new Node(back_image);
    back_left_node->modifier()->set_offset(Vector2D(VIDEO_X/2 - eye_image->width() * 0.5f - back_image->width(), VIDEO_Y - back_image->height()));

    node_->AddChild(eye_node);
    node_->AddChild(back_right_node);
    node_->AddChild(back_left_node);

    {   
        Node* eye_center = new Node;
        eye_center->modifier()->set_offset(Vector2D(eye_image->width() * 0.5f - 5.0f, eye_image->height() * 0.5f - 7.0f)); // The magic numbers do their magic!
        eye_center->AddChild(weapon_icon_ = new Node);

        eye_node->AddChild(eye_center);
    }


    Node* life_bar_container = new Node;
    life_bar_container->modifier()->set_offset(Vector2D(0.0f, VIDEO_Y - back_image->height()));
    life_bar_container->set_zindex(-0.5f);
    node_->AddChild(life_bar_container);

    Node* mana_bar_container = new Node;
    mana_bar_container->modifier()->set_offset(Vector2D(VIDEO_X - TOTEM_WIDTH, VIDEO_Y - back_image->height()));
    mana_bar_container->set_zindex(-0.5f);
    node_->AddChild(mana_bar_container);

    TexturedRectangle *life_bar_image = img_fac.LifeBarImage();
    TexturedRectangle *mana_bar_image = img_fac.ManaBarImage();

    Node* life_bar = CreateBarOn(life_bar_container, life_bar_image, img_fac);
    Node* mana_bar = CreateBarOn(mana_bar_container, mana_bar_image, img_fac);
    
    life_bar->AddChild(new Node(life_bar_image,  life_modifier_ = new Modifier));
    mana_bar->AddChild(new Node(img_fac.ManaBarImage(), block_modifier_ = new Modifier));
    mana_bar->AddChild(new Node(mana_bar_image,  mana_modifier_ = new Modifier));
    block_modifier_->set_alpha(0.75f);
    block_modifier_->set_color(ugdk::Color(0.5f, 0.5f, 0.5f));

    TexturedRectangle* mummy_counter_image = img_fac.MummyCounterImage();
    Node* mummy_counter_node = new Node(mummy_counter_image);
    mummy_counter_node->modifier()->set_offset(Vector2D(0.0f, back_image->height() - mummy_counter_image->height()));
    
    back_right_node->AddChild(mummy_counter_node);

    Node* mummy_number_node = new Node;
    mummy_number_node->modifier()->set_offset(Vector2D(mummy_counter_image->width() * 0.3f, mummy_counter_image->height() * 0.77f));

    mummy_counter_node->AddChild(mummy_number_node);

    previous_mummy_counter_value_ = world->CountRemainingEnemies();
    wchar_t str[8];
    swprintf(str, 8, L"%d", previous_mummy_counter_value_);
    enemy_counter_ = TEXT_MANAGER()->GetText(str);
    text_holder_ = new Node(enemy_counter_);
    text_holder_->modifier()->set_offset(Vector2D(enemy_counter_->width(), enemy_counter_->height()) * -0.5f);

    mummy_number_node->AddChild(text_holder_);
    
    /*
#ifdef DEBUG
    for(int i = 0; i < 3; ++i) {
        (fps_meter_[i] = new Sprite)->Initialize(number);
        fps_meter_[i]->set_position(Vector2D(
                        FPS_BAR_OFFSET_X + NUMBER_WIDTH*i + 0.0f, FPS_BAR_OFFSET_Y + 0.0f));
        //AddSprite(fps_meter_[i]);
        fps_meter_value_[i] = 0;
    }
#endif
    */
}

Hud::~Hud() {
    weapon_icon_->set_drawable(NULL);
    delete node_;
}

void Hud::Update(float delta_t) {
    World* world = WORLD();

    int enemy_number = world->CountRemainingEnemies();
    if(previous_mummy_counter_value_ != enemy_number) {
        previous_mummy_counter_value_ = enemy_number;

        delete enemy_counter_;

        wchar_t str[8];
        swprintf(str, 8, L"%d", enemy_number);

        enemy_counter_ = TEXT_MANAGER()->GetText(str);
        text_holder_->set_drawable(enemy_counter_);
        text_holder_->modifier()->set_offset(Vector2D(enemy_counter_->width(), enemy_counter_->height()) * -0.5f);
    }

    /*
#ifdef DEBUG
    int fps = Engine::reference()->current_fps();
    if(fps > 999) fps = 999;
    for(int i = 2; i >= 0; --i) {
        digit[i] = fps % 10;
        fps /= 10;
    }
    for(int i = 0; i < 3; ++i) {
        if(digit[i] != fps_meter_value_[i]) {
            fps_meter_value_[i] = digit[i];
            fps_meter_[i]->SetDefaultFrame(fps_meter_value_[i]);
        }
    }
#endif
    */
    if(world->hero() != NULL) {
        // Update the Selected weapon icon
        if(displayed_skill_ != world->hero()->secondary_combat_art()) {
            displayed_skill_ = world->hero()->secondary_combat_art();
            
            weapon_icon_->set_drawable(displayed_skill_->icon());
            if(displayed_skill_->icon() != NULL) {
                weapon_icon_->modifier()->set_offset(Vector2D(displayed_skill_->icon()->width() * (-0.5f), displayed_skill_->icon()->height() * (-0.5f) ));
            }
        }

        
        // Life Bar
        life_modifier_->set_offset(Vector2D(0.0f, -(((float) world->hero()->life()) / world->hero()->life().max_value()) * LIFE_BAR_HEIGHT) );
        
        // Mana Bar
        mana_modifier_->set_offset(Vector2D(0.0f, -(((float) world->hero()->mana()) / world->hero()->FullMana()) * MANA_BAR_HEIGHT) );

        block_modifier_->set_offset(Vector2D(0.0f, -(((float) world->hero()->mana_blocks().Get()) / world->hero()->mana_blocks().max_value()) * MANA_BAR_HEIGHT) );
    }
}

}
