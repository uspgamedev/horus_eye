//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/visionstrategy.h
// Definicao da classe LevelLoader.
//

#include"visionstrategy.h"
#include"geometryprimitives.h"
#include"../scenes/world.h"
#include"../sprites/hero.h"

using namespace std;
using namespace scene;
using namespace utils;
using namespace sprite;
using namespace framework;

bool solid(char obj){
    if(obj == 'W') return true;
    if(obj == 'D') return true;
    return false;
}

bool VisionStrategy::IsVisible(Vector2D position1, Vector2D position2){
    World *world = WORLD();
    vector<string> matrix = world->level_matrix();

    if(position2.x < 0.0){
        Hero* hero = world->hero();
        position2 = hero->world_position();
    }

    Vector2D distance = position2 - position1;
    if(distance.length() > Constants::MUMMY_SIGHT_RANGE)
        return false;

    for (int i = 0; i < (int)matrix.size(); i++) {
        for (int j = 0; j < (int)matrix[i].size(); j++) {
            if(solid(matrix[i][j])){
                float x = static_cast<float>(j);
                float y = static_cast<float>(matrix.size() - i - 1);

                Vector2D a = Vector2D(x - 0.5f, y - 0.5f);
                Vector2D b = Vector2D(x - 0.5f, y + 0.5f);
                Vector2D c = Vector2D(x + 0.5f, y + 0.5f);
                Vector2D d = Vector2D(x + 0.5f, y - 0.5f);
                if (GPintersect(a, b, position1, position2)) return false;
                if (GPintersect(b, c, position1, position2)) return false;
                if (GPintersect(c, d, position1, position2)) return false;
                if (GPintersect(d, a, position1, position2)) return false;
            }
        }
    }
    return true;
}

queue<Vector2D> VisionStrategy::Calculate(Vector2D position) {
    World *world = WORLD();
    Hero* hero = world->hero();

    queue<Vector2D> resp;

    if(hero == NULL)
        return resp;

    if(IsVisible(position,hero->world_position()))
        resp.push(hero->world_position());
    return resp;
}

