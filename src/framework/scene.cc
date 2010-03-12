/*
 * Scene.cpp
 *
 *  Created on: 04/02/2010
 *      Author: Kazuo
 */

#include "scene.h"

namespace framework {

using namespace std;

void Scene::Update(float delta_t) {

    list<Layer*>::iterator it = layers_.begin();

    while (it != layers_.end()) {
        (*it)->Update(delta_t);
        ++it;
    }
}

void Scene::Render() {

    list<Layer*>::iterator it = layers_.begin();

    while (it != layers_.end()) {
        (*it)->Render();
        ++it;
    }
}

}
