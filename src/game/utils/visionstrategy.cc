//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/visionstrategy.h
// Definicao da classe LevelLoader.
//

#include"visionstrategy.h"
#include"../scenes/world.h"
#include"../sprites/hero.h"
using namespace scene;
using namespace utils;
using namespace sprite;
using namespace framework;

// c esta a esquerda de a->b
bool left(Vector2D a, Vector2D b, Vector2D c){
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x) > 0;

}

bool intersect(Vector2D a, Vector2D b, Vector2D c, Vector2D d) {
    if ((left(a, b, c) != left(a, b, d)) && (left(c, d, a) != left(c, d, b))) return true;
    return false;
}

queue<Vector2D> VisionStrategy::Calculate(Vector2D position) {
    bool colision = false;
    World *world = ((World *)Engine::reference()->CurrentScene());
    char ** matrix = world->level_matrix();
    int width = world->level_width();
    int height = world->level_height();
    Hero* hero = world->hero();

    for (int i = 0; i < height && !colision; i++) {
        for (int j = 0; j < width && !colision; j++) {
            if (matrix[i][j] == 'W') {
                double x = j;
                double y = height - i - 1;

                Vector2D a = Vector2D(x - 0.5, y - 0.5);
                Vector2D b = Vector2D(x - 0.5, y + 0.5);
                Vector2D c = Vector2D(x + 0.5, y + 0.5);
                Vector2D d = Vector2D(x + 0.5, y - 0.5);
                if (intersect(a, b, position, hero->world_position())) colision = true;
                if (intersect(b, c, position, hero->world_position())) colision = true;
                if (intersect(c, d, position, hero->world_position())) colision = true;
                if (intersect(d, a, position, hero->world_position())) colision = true;
            }
        }
    }
    //Nao enxerga o heroi
    queue<Vector2D> resp;
    if(!colision)
        resp.push(hero->world_position());
    return resp;
}
