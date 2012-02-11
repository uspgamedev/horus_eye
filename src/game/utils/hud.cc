#include <ugdk/math/vector2D.h>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/modifier.h>
#include <ugdk/graphic/drawable/text.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>

#include "hud.h"

#include "game/scenes/world.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/constants.h"
#include "game/skills/skill.h"
#include "game/sprites/creatures/hero.h"

#define LIFE_BAR_HEIGHT Constants::LIFE_BAR_HEIGHT
#define MANA_BAR_HEIGHT Constants::MANA_BAR_HEIGHT

#define TOTEM_WIDTH Constants::TOTEM_WIDTH

#define VIDEO_Y VIDEO_MANAGER()->video_size().y 
#define VIDEO_X VIDEO_MANAGER()->video_size().x 

using namespace ugdk;
using namespace ugdk::graphic;
using scene::World;

namespace utils {

static Text* ConvertNumberToText(int val, bool center = true) {
    wchar_t str[15];
    swprintf(str, 15, L"%d", val);
    Text* result = TEXT_MANAGER()->GetText(str);
    if(center) result->set_hotspot(Drawable::CENTER);
    return result;
}

static Node* CreateBarOn(Node* container, TexturedRectangle* bar_image, HudImageFactory& img_fac) {

    TexturedRectangle 
        *totem_bar_image = img_fac.TotemImage(),
        *totem_bar_bottom = img_fac.TotemBottomImage();

    totem_bar_image->set_hotspot(Drawable::BOTTOM_LEFT);
    totem_bar_bottom->set_hotspot(Drawable::BOTTOM_LEFT);

    Node* totem_bottom_node = new Node(totem_bar_bottom);
    totem_bottom_node->set_zindex(1.0);
    container->AddChild(totem_bottom_node);

    Node* totem_top_node = new Node(totem_bar_image);
    totem_top_node->modifier()->set_offset(Vector2D(0.0, static_cast<double>(-totem_bar_bottom->height())));
    totem_top_node->set_zindex(-1.0);
    container->AddChild(totem_top_node);

    Node* bar_node = new Node;
    bar_node->modifier()->set_offset(Vector2D(totem_bar_image->width() * 0.5 - bar_image->width() * 0.5, totem_bar_bottom->height() * (-1.0)));
    bar_node->set_zindex(-0.5);
    container->AddChild(bar_node);

    return bar_node;
}

// Aviso: maximo de 999 mumias no display de inimigos restantes!
Hud::Hud(World* world) : node_(new Node), displayed_skill_(NULL) {
    // Criando sprites da life bar
    HudImageFactory img_fac;

    TexturedRectangle* back_left_image = img_fac.BackImage();
    TexturedRectangle* back_right_image = img_fac.BackImage();
    TexturedRectangle* eye_image = img_fac.EyeImage();

    eye_image->set_hotspot(Drawable::BOTTOM);
    back_left_image->set_hotspot(Drawable::BOTTOM_RIGHT);
    back_right_image->set_hotspot(Drawable::BOTTOM_LEFT);

    Node *bottom_node, *eye_node, *back_right_node, *back_left_node;

    // Hook node for the bottom of the screen.
    node_->AddChild(bottom_node = new Node);
    bottom_node->modifier()->set_offset(Vector2D(VIDEO_X/2, VIDEO_Y));


    bottom_node->AddChild(back_right_node = new Node(back_right_image));
    bottom_node->AddChild(back_left_node  = new Node(back_left_image ));

    back_left_node ->modifier()->set_offset(Vector2D(eye_image->width() * -0.5, 0.0));
    back_right_node->modifier()->set_offset(Vector2D(eye_image->width() *  0.5, 0.0));

    back_left_node->set_zindex(0.1);
    back_right_node->set_zindex(0.1);


    // The eye image is the bottom of the screen
    bottom_node->AddChild(eye_node = new Node(eye_image));

    // weapon_icon_ is a hook to the middle of the eye image in the bottom
    weapon_icon_ = new Node;

    // The magic numbers do their magic!
    weapon_icon_->modifier()->set_offset(Vector2D(-5.0, -eye_image->height() * 0.5 - 7.0));
    eye_node->AddChild(weapon_icon_);


    Node* life_bar_container = new Node;
    life_bar_container->modifier()->set_offset(Vector2D(0.0, VIDEO_Y - back_left_image->height()));
    life_bar_container->set_zindex(-0.5);
    node_->AddChild(life_bar_container);

    Node* mana_bar_container = new Node;
    mana_bar_container->modifier()->set_offset(Vector2D(VIDEO_X - TOTEM_WIDTH, VIDEO_Y - back_right_image->height()));
    mana_bar_container->set_zindex(-0.5);
    node_->AddChild(mana_bar_container);

    TexturedRectangle *life_bar_image = img_fac.LifeBarImage();
    TexturedRectangle *mana_bar_image = img_fac.ManaBarImage();

    Node* life_bar = CreateBarOn(life_bar_container, life_bar_image, img_fac);
    Node* mana_bar = CreateBarOn(mana_bar_container, mana_bar_image, img_fac);
    
    life_bar->AddChild(new Node(life_bar_image,  life_modifier_ = new Modifier));
    mana_bar->AddChild(new Node(img_fac.ManaBarImage(), block_modifier_ = new Modifier));
    mana_bar->AddChild(new Node(mana_bar_image,  mana_modifier_ = new Modifier));
    block_modifier_->set_alpha(0.75);
    block_modifier_->set_color(ugdk::Color(0.5, 0.5, 0.5));


    TexturedRectangle* mummy_counter_image = img_fac.MummyCounterImage();
    mummy_counter_image->set_hotspot(Drawable::BOTTOM_LEFT);


    Node *mummy_counter_node;

    // Background image for the mummy counter
    back_right_node->AddChild(mummy_counter_node = new Node(mummy_counter_image));

    mummy_counter_node->AddChild(mummy_counter_text_holder_ = new Node);
    mummy_counter_text_holder_->modifier()->set_offset(
        Vector2D(mummy_counter_image->width() * 0.3, 
                -mummy_counter_image->height() * (1 - 0.77)));

    previous_mummy_counter_value_ = world->CountRemainingEnemies();
    mummy_counter_text_holder_->set_drawable(ConvertNumberToText(previous_mummy_counter_value_));
    
#ifdef DEBUG
    Text* fps_label = TEXT_MANAGER()->GetText(L"FPS: ");
    node_->AddChild(new Node(fps_label));
    node_->AddChild(fps_meter_node_ = new Node(ConvertNumberToText(0)));
    fps_meter_node_->modifier()->set_offset(Vector2D(fps_label->width(), 0.0));
    previous_fps_ = 0;
#endif
}

Hud::~Hud() {
    weapon_icon_->set_drawable(NULL);
    delete node_;
}

void Hud::Update(double delta_t) {
    World* world = WORLD();

    int enemy_number = world->CountRemainingEnemies();
    if(previous_mummy_counter_value_ != enemy_number) {
        previous_mummy_counter_value_ = enemy_number;

        delete mummy_counter_text_holder_->drawable();
        mummy_counter_text_holder_->set_drawable(ConvertNumberToText(enemy_number));
    }

#ifdef DEBUG
    int fps = Engine::reference()->current_fps();
    if(std::abs(previous_fps_ - fps) > 0) {
        previous_fps_ = fps;
        delete fps_meter_node_->drawable();
        fps_meter_node_->set_drawable(ConvertNumberToText(fps, false));
    }
#endif
    if(world->hero() != NULL) {
        // Update the Selected weapon icon
        if(displayed_skill_ != world->hero()->secondary_combat_art()) {
            displayed_skill_ = world->hero()->secondary_combat_art();
            
            weapon_icon_->set_drawable(displayed_skill_->icon());
            if(displayed_skill_->icon() != NULL)
                displayed_skill_->icon()->set_hotspot(Drawable::CENTER);
        }

        
        // Life Bar
        life_modifier_->set_offset(Vector2D(0.0, -(((double) world->hero()->life()) / world->hero()->life().max_value()) * LIFE_BAR_HEIGHT) );
        
        // Mana Bar
        mana_modifier_->set_offset(Vector2D(0.0, -(((double) world->hero()->mana()) / world->hero()->FullMana()) * MANA_BAR_HEIGHT) );

        block_modifier_->set_offset(Vector2D(0.0, -(((double) world->hero()->mana_blocks().Get()) / world->hero()->mana_blocks().max_value()) * MANA_BAR_HEIGHT) );
    }
}

}
