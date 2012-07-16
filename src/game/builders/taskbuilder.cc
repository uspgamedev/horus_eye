#include <list>
#include <ugdk/portable/tr1.h>
#include FROM_TR1(functional)
#include <ugdk/action/generictask.h>
#include <ugdk/base/engine.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/ui/menu.h>
#include <ugdk/math/vector2D.h>

#include "taskbuilder.h"

#include "game/scenes/menu.h"
#include "game/builders/goodmenubuilder.h"
#include "game/sprites/worldobject.h"
#include "game/utils/tile.h"
#include "game/utils/visionstrategy.h"

using std::tr1::bind;
using namespace std::tr1::placeholders;
using ugdk::Vector2D;
using utils::Tile;
using utils::TilePos;
using utils::GameMap;

static bool VerifyPause(double dt) {
    ugdk::input::InputManager *input = ugdk::Engine::reference()->input_manager();
    if(input->KeyPressed(ugdk::input::K_ESCAPE)) {
        builder::MenuBuilder builder;
        ugdk::Engine::reference()->PushScene(builder.PauseMenu());
    }
    return true;
}

static bool IsNear(const TilePos &origin, const TilePos &pos, double radius) {
    if ((double)(abs((pos.i - origin.i)) + abs((pos.j - origin.j))) <= radius)
        return true;
    else if ((Tile::FromTilePos(pos) - Tile::FromTilePos(origin)).length() <= radius )
        return true;
    else return false;
}

class UpdateVisibility : public ugdk::action::Task {
  public:
    UpdateVisibility(sprite::WorldObject* _hero, GameMap& _map)
        : hero(_hero), map(_map) {}

    void operator()(double dt) {
        if(!hero->is_active()) return;

        TilePos hero_pos = Tile::ToTilePos(hero->world_position());

        hero_pos.i = static_cast<int>(map.size()) - hero_pos.i - 1;

        Tile::CleanVisibility(map);
        SpreadLight(hero_pos, 1.5*hero->light_radius());
    }

  private:
    void SpreadLight(const TilePos &origin_pos, double radius) {
        std::list<Tile*>     queue;
        Vector2D        origin_world_pos = Tile::FromTilePos(origin_pos);
        Tile            *origin = Tile::GetFromMapPosition(map, origin_pos);
        utils::VisionStrategy  vision;

        if (!origin) return;

        origin_world_pos.y = map.size() - origin_world_pos.y - 1;
        queue.push_back(origin);

        while (queue.size() > 0) {
            Tile *tile = queue.front();
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
                    tile->floor()->modifier()->set_color(ugdk::Color());
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

    sprite::WorldObject* hero;
    GameMap& map;
};


namespace builder {

ugdk::action::Task* TaskBuilder::PauseMenuTask() const {
    return new ugdk::action::GenericTask(VerifyPause);
}

ugdk::action::Task* TaskBuilder::VisibilityTask(sprite::WorldObject* hero, GameMap& map) const {
    return new UpdateVisibility(hero, map);
}

}
