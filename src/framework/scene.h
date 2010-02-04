/*
 * Scene.h
 *
 *  Created on: 04/02/2010
 *      Author: Kazuo
 */

#ifndef HORUSEYE_FRAMEWORK_SCENE_H_
#define HORUSEYE_FRAMEWORK_SCENE_H_

#include "layer.h"
#include <list>

namespace framework {

class Scene {
  public:
    // The Evil Constructor!
    Scene() {
        // TODO Auto-generated constructor stub

    }
    // The Good Destructor!
    virtual ~Scene();

    // Funcoes basicas pra adicionar e remover layers...
    void AddLayer(Layer *layer) { layers_.push_back(layer); };
    void RemoveLayer(Layer *layer) { layers_.remove(layer); };

    // Atualiza a logica da cena...
    virtual void Update(float delta_t);

    // Redesenha o grafico da cena...
    void Render();

  protected:
    ::std::list<Layer*> layers_;
};

}

#endif /* HORUSEYE_FRAMEWORK_SCENE_H_ */
