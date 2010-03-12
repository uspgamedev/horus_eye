/*
 * layer.cc
 *
 *  Created on: 04/02/2010
 *      Author: Kazuo
 */

#include "layer.h"

namespace framework {

using namespace std;

void Layer::Update(float delta_t) {

    list<Sprite*>::iterator it = sprite_list_.begin();

    while (it != sprite_list_.end()) {
        (*it)->Update(delta_t);
        ++it;
    }
}

void Layer::Render() {

    list<Sprite*>::iterator it = sprite_list_.begin();

        while (it != sprite_list_.end()) {
            //Preciso de um objeto VideoManager!
            //(*it).render(back_buffer);
            ++it;
        }
}

}
