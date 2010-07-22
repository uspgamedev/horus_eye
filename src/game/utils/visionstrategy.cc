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
    World *world = ((World *)Engine::reference()->CurrentScene());
    char ** matrix = world->level_matrix();
    int width = world->level_width();
    int height = world->level_height();

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if(solid(matrix[i][j])){
                double x = j;
                double y = height - i - 1;

                Vector2D a = Vector2D(x - 0.5, y - 0.5);
                Vector2D b = Vector2D(x - 0.5, y + 0.5);
                Vector2D c = Vector2D(x + 0.5, y + 0.5);
                Vector2D d = Vector2D(x + 0.5, y - 0.5);
                if (GPintersect(a, b, position1, position2)) return true;
                if (GPintersect(b, c, position1, position2)) return true;
                if (GPintersect(c, d, position1, position2)) return true;
                if (GPintersect(d, a, position1, position2)) return true;
            }
        }
    }
    return false;
}

queue<Vector2D> VisionStrategy::Calculate(Vector2D position) {
    World *world = ((World *)Engine::reference()->CurrentScene());
    Hero* hero = world->hero();

    queue<Vector2D> resp;

    if(hero == NULL)
        return resp;

    if(IsVisible(position,hero->world_position()))
        resp.push(hero->world_position());
    return resp;
}

