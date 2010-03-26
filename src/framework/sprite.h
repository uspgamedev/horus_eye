//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/sprite.h
// Definicao da classe Sprite.
//
#ifndef HORUSEYE_FRAMEWORK_SPRITE_H_
#define HORUSEYE_FRAMEWORK_SPRITE_H_

#include "animation.h"
#include "image.h"
#include "vector2D.h"


namespace framework {

class Sprite {
  public:
    Sprite(Image *image);
    Sprite(Image *image, Vector2D position);
    virtual ~Sprite() { }


    // Acessors e mutators
    Vector2D position() const { return position_; }
    void set_position(Vector2D position) { position_ = position; }

    bool visible() const { return visible_; }
    void set_visible(bool visible) { visible_ = visible; }

    Image *image() { return image_; }


    // ======================================================================
    // Substituimos o sistema de guardar animacoes no sprite por simplesmente
    // atribuir diretamente a animacao desejada.
    // ======================================================================

    // Devole um numero negativo caso nao seja possível registrar a animacao.
    // Caso contrario, devolve um identificador positivo univoco.
    //int RegisterAnimation(Animation *animation);

    // Recebe um identificador da animacao
    // Devole true quando a animação for desregistrada e false caso contrario
    //bool UnregisterAnimation(int animation_id);

    // Recebe um identificador da animacao
    // Devole true quando a animação for selecionada e false caso contrario
    //bool SelectAnimation(int animation_id);

    // ======================================================================
    void SelectAnimation(Animation *animation);

    void Render(Image *back_buffer);

    // Realiza a atualizacao com base na variacao de tempo (delta_t)
    virtual void Update(float delta_t);

    void Initialise(Image *image);

    Image::Mirror mirror() const { return mirror_; }
    void set_mirror(Image::Mirror mirror) { mirror_ = mirror; }

  protected:
    Vector2D position_;
    Image *image_;
    Image::Mirror mirror_;
    Animation *animation_;
    bool visible_;

};
}

#endif //HORUSEYE_FRAMEWORK_SPRITE_H_
