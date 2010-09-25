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

#define TO_MATRIX(value) static_cast<int>(value + 0.5f)

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

    Vector2D ipos1(TO_MATRIX(position1.x), matrix.size() - TO_MATRIX(position1.y) - 1);
    Vector2D ipos2(TO_MATRIX(position2.x), matrix.size() - TO_MATRIX(position2.y) - 1);
    Vector2D dir = ipos2 - ipos1;
    int step_i = dir.y >= 0 ? 1 : -1,
        step_j = dir.x >= 0 ? 1 : -1;
    int height = max(dir.y >= 0 ? (int)dir.y : (int)-dir.y, 1);
    int proportion = abs(dir.y != 0 ? (int)(dir.x/dir.y) : (int)(dir.x));
    int k = (int)ipos1.x;
    int i, j = k;

    /*printf("pos1=[%d][%d]\n", (int)ipos1.y, (int)ipos1.x);
    printf("pos2=[%d][%d]\n", (int)ipos2.y, (int)ipos2.x);
    printf("height=%d\nproportion=%d\n", height, proportion);*/
    for (int di = 0; di < height; di++) {
        for (int dj = 0; dj <= proportion; dj++) {
            i = ipos1.y + di*step_i;
            j = k + dj*step_j;
            //printf("Checking [%d][%d] --> %c\n", i, j, matrix[i][j]);
            if(solid(matrix[i][j]))
                return false;
        }
        if (proportion > 0)
            k = j;
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

