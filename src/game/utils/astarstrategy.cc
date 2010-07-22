//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/astarstrategy.h
// Definicao da classe AStarStrategy.
//
#include<set>
#include"astarstrategy.h"
#include"../scenes/world.h"
#include"../sprites/hero.h"
#include"visionstrategy.h"
#include<cmath>
using namespace scene;
using namespace utils;
using namespace sprite;
using namespace framework;
using namespace std;

int dx[] = {0,0,1,-1,1,-1,1,-1};
int dy[] = {-1,1,0,0,1,-1,-1,1};
int width,height;
char **matrix;
struct vertex{
    int x;
    int y;
    int cst;
};

bool operator < (vertex a, vertex b){
    if(a.cst != b.cst) return a.cst < b.cst;
    if(a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

vertex make_vertex(int x,int y,int cst){
    vertex v;
    v.x = x;
    v.y = y;
    v.cst = cst;
    return v;
}
bool SOLID(char x){
    if(x == 'W' || x == 'D') return true;
    return false;
}

double DIST(int ax,int ay,int bx,int by){
    return sqrt((ax-bx)*(ax-bx) + (ay-by)*(ay-by));
}


bool valid(int x,int y){
    int i = height - y - 1;
    int j = x;
    if(i < 0 || j < 0 || i > height || j > width) return false;
    if(SOLID(matrix[i][j])) return false;
    return true;
}
queue<Vector2D> AStarStrategy::Calculate(Vector2D position) {
    World *world = ((World *)Engine::reference()->CurrentScene());
    Hero* hero = world->hero();
    matrix = world->level_matrix();
    width = world->level_width();
    height = world->level_height();

    double dist[width][height];
    for(int i = 0;i < width;i++)
        for(int j = 0;j < height;j++)
            dist[i][j] = -1.0;

    vertex parnt[width][height];

    vertex target = make_vertex(position.x+0.5,position.y+0.5,0);
    vertex source = make_vertex(hero->world_position().x+0.5,hero->world_position().y+0.5,0);

    set<vertex> priority_queue;
    priority_queue.insert(source);

    dist[source.x][source.y] = 0;
    parnt[source.x][source.y] = make_vertex(0,0,-1);

    while(priority_queue.size() > 0){
        vertex v = *(priority_queue.begin());
        priority_queue.erase(priority_queue.begin());
        if(v.x == target.x && v.y == target.y) break;

        for(int d = 0;d < 8;d++){
            int xx = v.x + dx[d];
            int yy = v.y + dy[d];

            if(valid(xx,yy) && (dist[xx][yy] == -1 || dist[xx][yy] > v.cst + DIST(xx,yy,v.x,v.y))){
                dist[xx][yy] = v.cst + DIST(xx,yy,v.x,v.y);
                parnt[xx][yy] = v;
                priority_queue.insert(make_vertex(xx,yy,dist[xx][yy]));
            }
        }
    }
    queue<Vector2D> resp;

//    VisionStrategy vision;
    resp.push(Vector2D(target.x,target.y));
    for(vertex w = target, v = parnt[w.x][w.y]; v.cst >= 0; w = v, v = parnt[source.x][source.y]){
        resp.push(Vector2D(v.x,v.y));
    }
    return resp;
}
