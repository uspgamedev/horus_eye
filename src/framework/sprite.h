#ifndef HORUSEYE_FRAMEWORK_SPRITE_H_
#define HORUSEYE_FRAMEWORK_SPRITE_H_

#include <algorithm>
#include "image.h"
#include "vector2D.h"
#include "types.h"
#include "animation.h"


namespace framework {

class Drawable;
//class Animation;
class Modifier;
class Light;
class AnimationSet;
class Sprite {
  public:
    Sprite();
    virtual ~Sprite();

    virtual void Initialize(Drawable *image, AnimationSet *set = NULL,
                    bool delete_image = false);

    // Acessors e mutators
    Vector2D position() const { return position_; }
    void set_position(const Vector2D& position) { position_ = position; }

    bool visible() const { return visible_; }
    void set_visible(bool visible) { visible_ = visible; }

    Vector2D hotspot() const { return hotspot_; }
    void set_hotspot(const Vector2D& hotspot) { hotspot_ = hotspot; }

    // zindex: (tras) 0.0 <= zindex <= 1.0 (frente)
    float zindex() const { return zindex_; }
    void set_zindex(float z) { zindex_= z; }
    static bool CompareByZIndex(Sprite *a, Sprite *b);

    //Image *image() { return image_; }

	Color color() {
	    return modifier_ ? modifier_->color() : WHITE;
	}
	void set_color(Color color) {
	    if(modifier_) modifier_->set_color(color);
	}
	
	float alpha() {
	    return modifier_ ? modifier_->alpha() : 1.0f;
	}
	void set_alpha(float alpha) {
	    if (modifier_) modifier_->set_alpha(alpha);
	}

	const Vector2D size() { return size_; }
	void set_size(const Vector2D& size) { size_ = size; }
	
	Light* light() { return light_; }
	void set_light(Light* light) { light_ = light; }


    // ======================================================================
    // Substituimos o sistema de guardar animacoes no sprite por simplesmente
    // atribuir diretamente a animacao desejada.
    // ======================================================================

    // Devolve um numero negativo caso nao seja possível registrar a animacao.
    // Caso contrario, devolve um identificador positivo univoco.
    //int RegisterAnimation(Animation *animation);

    // Recebe um identificador da animacao
    // Devole true quando a animação for desregistrada e false caso contrario
    //bool UnregisterAnimation(int animation_id);

    // Recebe um identificador da animacao
    // Devole true quando a animação for selecionada e false caso contrario
    //bool SelectAnimation(int animation_id);

    // ======================================================================
    //void SelectAnimation(Animation *animation);
    void SelectAnimation(std::string animation_name) {
        animation_->Select(animation_name);
    }
    void SelectAnimation(int animation_index) {
        animation_->Select(animation_index);
    }
    void SetAnimationFPS(int fps) { animation_->set_fps(fps); }
    float GetAnimationFPS() const { return animation_->fps(); }
    void AddObserverToAnimation(Observer *observer) {
        animation_->AddObserver(observer);
    }
    int GetAnimationFrameNumber() const { return animation_->n_frames(); }
    //Animation* animation() { return animation_; }

    virtual void Render();
	void RenderLight(Vector2D &offset);

    // Realiza a atualizacao com base na variacao de tempo (delta_t)
    virtual void Update(float delta_t);

  protected:
    Mirror mirror() const { return modifier_->mirror(); }
    void set_mirror(Mirror mirror) { modifier_->set_mirror(mirror); }

	Light *light_;
    float zindex_;

  private:
    Vector2D position_, hotspot_, size_;
    //Image *image_;
	Drawable *image_;
    //Image::Mirror mirror_;
	//Color color_;
	//float alpha_;
    Animation *animation_;
    Modifier *modifier_;
    bool visible_, delete_image_;

};
}

#endif //HORUSEYE_FRAMEWORK_SPRITE_H_
