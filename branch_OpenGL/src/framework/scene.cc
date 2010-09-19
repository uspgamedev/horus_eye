//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/scene.c
// Implementacao da classe Scene.
//

#include "scene.h"

namespace framework {

using namespace std;

Scene::~Scene() {
    if (layers_.size() == 0) return;
    list<Layer*>::iterator it = layers_.begin();
    while (it != layers_.end()) {
        delete (*it);
        ++it;
    }
}

void Scene::Update(float delta_t) {

    if (layers_.size() == 0) return;

    list<Layer*>::iterator it = layers_.begin();

    while (it != layers_.end()) {
        (*it)->Update(delta_t);
        ++it;
    }
}

void Scene::Render() {

    if (visible_) {
        list<Layer*>::iterator it = layers_.begin();

        while (it != layers_.end()) {
            (*it)->Render();
            ++it;
        }
    }
}

}