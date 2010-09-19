//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/layer.h
// Definicao da classe Layer.
//

#ifndef HORUSEYE_FRAMEWORK_LAYER_H_
#define HORUSEYE_FRAMEWORK_LAYER_H_

#include <vector>
#include "vector2D.h"
#include "sprite.h"

namespace framework {

// Representa uma camada grafica na qual os Sprites sao desenhados.
//
// Um objeto Layer pode ter um offset, que e' o deslocamento da camada inteira
// com relacao 'a origem da tela(fullscreen) ou janela.
//
class Layer {
  public:
    // Construtores e destrutores.
    Layer(Vector2D offset = Vector2D()) : visible_(true) {}
    virtual ~Layer();

    // Acesso e leitura de atributos.
    bool IsVisible() { return visible_; }
    void set_visible(bool visible) { visible_ = visible; }
    void set_offset(Vector2D offset) { offset_ = offset; }
    Vector2D offset() { return offset_; }

    // Adicionando e removendo Sprites.
    void AddSprite(Sprite *sprite);
    void RemoveSprite(Sprite *sprite);

    // Atualiza a camada e seus Sprites.
    virtual void Update(float delta_t);

    // Desenha a camada.
    virtual void Render();

  protected:
    // Estrutura basica para armazenar os Sprites. Nao definitivo.
    std::vector<Sprite*> sprite_list_;
    bool visible_;

  private:
    // Atributos.
    Vector2D offset_;

    // Ordena os sprites pelo zindex
    virtual void SortSprites();

}; // class Layer.

}

#endif // HORUSEYE_FRAMEWORK_LAYER_H_