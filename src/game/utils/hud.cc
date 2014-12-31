#include "hud.h"

#include <ugdk/math/vector2D.h>
#include <ugdk/system/engine.h>
#include <ugdk/text/module.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/ui/node.h>
#include <ugdk/ui/drawable/texturedrectangle.h>
#include <ugdk/text/label.h>
#include <ugdk/time/module.h>

#include "game/config.h"
#include "game/core/world.h"
#include "game/utils/hudimagefactory.h"
#include "game/skills/skill.h"
#include "game/sprites/worldobject.h"
#include "game/components/damageable.h"
#include "game/components/controller.h"
#include "game/components/caster.h"
#include "game/constants.h"

#include <string>
#include <sstream>

#define LIFE_BAR_HEIGHT constants::GetDouble("LIFE_BAR_HEIGHT")
#define MANA_BAR_HEIGHT constants::GetDouble("MANA_BAR_HEIGHT")

#define TOTEM_WIDTH constants::GetDouble("TOTEM_WIDTH")

#define VIDEO_Y ugdk::graphic::manager()->screen()->size().y
#define VIDEO_X ugdk::graphic::manager()->screen()->size().x

using namespace ugdk;
using namespace ugdk::ui;
using core::World;
using component::Controller;

namespace utils {

static std::unique_ptr<text::Label> ConvertNumberToText(int val, bool center = true) {
#ifdef ANDROID
    std::ostringstream convert;
    convert << val;
    std::string s = convert.str();
#else
    std::string s = std::to_string(val);
#endif
    auto result = ugdk::MakeUnique<text::Label>(s, ugdk::text::manager()->current_font());
    if(center) result->set_hotspot(HookPoint::CENTER);
    return std::move(result);
}

static std::shared_ptr<Node> CreateBarHolderOn(std::shared_ptr<Node> container, TexturedRectangle* bar_image, HudImageFactory& img_fac) {

    auto
        totem_bar_image(img_fac.TotemImage()),
        totem_bar_bottom(img_fac.TotemBottomImage());

    totem_bar_image->set_hotspot(HookPoint::BOTTOM_LEFT);
    totem_bar_bottom->set_hotspot(HookPoint::BOTTOM_LEFT);

    auto totem_bottom_node = std::make_shared<Node>(std::unique_ptr<Drawable>(totem_bar_bottom));
    totem_bottom_node->set_zindex(1.0);
    container->AddChild(totem_bottom_node);

    auto totem_top_node = std::make_shared<Node>(std::unique_ptr<Drawable>(totem_bar_image));
    totem_top_node->geometry().set_offset(Vector2D(0.0, static_cast<double>(-totem_bar_bottom->height())));
    totem_top_node->set_zindex(-1.0);
    container->AddChild(totem_top_node);

    auto bar_node = std::make_shared<Node>();
    bar_node->geometry().set_offset(Vector2D(totem_bar_image->width() * 0.5 - bar_image->width() * 0.5,
                                             totem_bar_bottom->height() * (-1.0)));
    bar_node->set_zindex(-0.5);
    container->AddChild(bar_node);

    return bar_node;
}

// Aviso: maximo de 999 mumias no display de inimigos restantes!
Hud::Hud(World* world) 
: world_(world)
, node_(new Node)
, displayed_skill_(nullptr)
{
    // Criando sprites da life bar
    HudImageFactory img_fac;

    TexturedRectangle* back_left_image = (img_fac.BackImage());
    TexturedRectangle* back_right_image = (img_fac.BackImage());
    TexturedRectangle* eye_image = (img_fac.EyeImage());

    eye_image->set_hotspot(HookPoint::BOTTOM);
    back_left_image->set_hotspot(HookPoint::BOTTOM_RIGHT);
    back_right_image->set_hotspot(HookPoint::BOTTOM_LEFT);

    std::shared_ptr<Node> bottom_node, eye_node, back_right_node, back_left_node;

    // Hook node for the bottom of the screen.
    node_->AddChild(bottom_node = std::make_shared<Node>());
    bottom_node->geometry().set_offset(Vector2D(VIDEO_X/2, VIDEO_Y));


    bottom_node->AddChild(back_right_node = std::make_shared<Node>(std::unique_ptr<Drawable>(back_right_image)));
    bottom_node->AddChild(back_left_node = std::make_shared<Node>(std::unique_ptr<Drawable>(back_left_image)));

    back_left_node ->geometry().set_offset(Vector2D(eye_image->width() * -0.5, 0.0));
    back_right_node->geometry().set_offset(Vector2D(eye_image->width() *  0.5, 0.0));

    back_left_node->set_zindex(0.1);
    back_right_node->set_zindex(0.1);


    // The eye image is the bottom of the screen
    bottom_node->AddChild(eye_node = std::make_shared<Node>(std::unique_ptr<Drawable>(eye_image)));

    // weapon_icon_ is a hook to the middle of the eye image in the bottom
    weapon_icon_ = std::make_shared<Node>();

    // The magic numbers do their magic!
    weapon_icon_->geometry().set_offset(Vector2D(-5.0, -eye_node->drawable()->height() * 0.5 - 7.0));
    eye_node->AddChild(weapon_icon_);


    auto life_bar_container = std::make_shared<Node>();
    life_bar_container->geometry().set_offset(Vector2D(0.0, VIDEO_Y - back_left_image->height()));
    life_bar_container->set_zindex(-0.5);
    node_->AddChild(life_bar_container);

    auto mana_bar_container = std::make_shared<Node>();
    mana_bar_container->geometry().set_offset(Vector2D(VIDEO_X - TOTEM_WIDTH, VIDEO_Y - back_right_image->height()));
    mana_bar_container->set_zindex(-0.5);
    node_->AddChild(mana_bar_container);

    std::unique_ptr<TexturedRectangle> life_bar_image(img_fac.LifeBarImage());
    std::unique_ptr<TexturedRectangle> mana_bar_image(img_fac.ManaBarImage());

    auto life_bar_holder = CreateBarHolderOn(life_bar_container, life_bar_image.get(), img_fac);
    auto mana_bar_holder = CreateBarHolderOn(mana_bar_container, mana_bar_image.get(), img_fac);
    
    life_bar_holder->AddChild(life_bar_ = std::make_shared<Node>(std::move(life_bar_image)));
    mana_bar_holder->AddChild(block_bar_ = std::make_shared<Node>(std::unique_ptr<TexturedRectangle>(img_fac.ManaBarImage())));
    mana_bar_holder->AddChild(mana_bar_ = std::make_shared<Node>(std::move(mana_bar_image)));

    block_bar_->effect().set_color(ugdk::Color(0.5, 0.5, 0.5, 0.75));

    auto mummy_counter_image(img_fac.MummyCounterImage());
    mummy_counter_image->set_hotspot(HookPoint::BOTTOM_LEFT);

    std::shared_ptr<Node> mummy_counter_node;

    // Background image for the mummy counter
    back_right_node->AddChild(mummy_counter_node = std::make_shared<Node>(std::unique_ptr<Drawable>(mummy_counter_image)));

    mummy_counter_node->AddChild(mummy_counter_text_holder_ = std::make_shared<Node>());
    mummy_counter_text_holder_->geometry().set_offset(
        Vector2D(mummy_counter_image->width() * 0.3, 
                -mummy_counter_image->height() * (1 - 0.77)));

    //previous_mummy_counter_value_ = world->CountRemainingEnemies();
    mummy_counter_text_holder_->set_drawable(ConvertNumberToText(previous_mummy_counter_value_));
    
#ifdef HORUSEYE_DEBUG_TOOLS

    auto fps_node = std::make_shared<Node>();

    auto fps_label(new text::Label("FPS: ", ugdk::text::manager()->current_font()));
    fps_node->AddChild(std::make_shared<Node>(std::unique_ptr<Drawable>(fps_label)));
    fps_node->AddChild(fps_meter_node_ = std::make_shared<Node>(ConvertNumberToText(0)));
    fps_meter_node_->geometry().set_offset(Vector2D(fps_label->width(), 0.0));
    previous_fps_ = 0;

    fps_node->geometry().set_offset(Vector2D(VIDEO_X - fps_label->width() * 3, 0));
    node_->AddChild(fps_node);
#endif
}

Hud::~Hud() {
    //weapon_icon_->set_drawable(nullptr);
}

void Hud::Update(double delta_t) {
    int enemy_number = 0;//world->CountRemainingEnemies();
    if(previous_mummy_counter_value_ != enemy_number) {
        previous_mummy_counter_value_ = enemy_number;

        mummy_counter_text_holder_->set_drawable(ConvertNumberToText(enemy_number));
    }

#ifdef HORUSEYE_DEBUG_TOOLS
    double fps = 1000.0/ugdk::time::manager()->TimeDifference();
    if(std::abs(previous_fps_ - fps) > 0.5) {
        previous_fps_ = fps;
        fps_meter_node_->set_drawable(ConvertNumberToText(static_cast<int>(fps), false));
    }
#endif

    sprite::WObjPtr hero = world_->hero().lock();
    life_bar_->effect().set_visible(hero && hero->damageable());
    mana_bar_->effect().set_visible(hero && hero->caster());
    if(hero) {
        // Life Bar
        if(hero->damageable())
            life_bar_->geometry().set_offset(Vector2D(0.0, -hero->damageable()->life().percentage() * LIFE_BAR_HEIGHT));

        if(hero->caster()) {
            component::Caster* caster = hero->caster();
            // Update the Selected weapon icon
            if(displayed_skill_ != caster->SkillAt(Controller::SECONDARY)) {
                displayed_skill_ = caster->SkillAt(Controller::SECONDARY);
                
                weapon_icon_->set_drawable(displayed_skill_->CreateIcon());
                if (weapon_icon_->drawable())
                    weapon_icon_->drawable()->set_hotspot(HookPoint::CENTER);
            }
            // Mana Bar
            mana_bar_->geometry().set_offset(Vector2D(0.0, 
                -(caster->mana().Get() / caster->FullMana()) * MANA_BAR_HEIGHT) );
            block_bar_->geometry().set_offset(Vector2D(0.0, 
                -(((double) caster->mana_blocks().Get()) / caster->mana_blocks().max_value()) * MANA_BAR_HEIGHT) );
        }

    }
}

}
