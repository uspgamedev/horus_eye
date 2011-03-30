#ifndef HORUSEYE_FRAMEWORK_SPRITE_H_
#define HORUSEYE_FRAMEWORK_SPRITE_H_

#include <algorithm>
#include "image.h"
#include "vector2D.h"
#include "types.h"


namespace framework {

class Drawable;
class Animation;
class Light;
class Sprite {
  public:
    Sprite();
    virtual ~Sprite();

    void Initialize(Image *image, bool delete_image = false);

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

	Color color() { return color_; }
	void set_color(Color color) { color_ = color; }
	
	float alpha() { return alpha_; }
	void set_alpha(float alpha) { alpha_ = alpha; }

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
    void SelectAnimation(Animation *animation);
    Animation* animation() { return animation_; }

    virtual void Render(Image *back_buffer, Vector2D &offset);
	void RenderLight(Vector2D &offset);

    // Realiza a atualizacao com base na variacao de tempo (delta_t)
    virtual void Update(float delta_t);

  protected:
    Image::Mirror mirror() const { return mirror_; }
    void set_mirror(Image::Mirror mirror) { mirror_ = mirror; }
    bool withinScreen(Vector2D &offset);

	Light *light_;
    float zindex_;

  private:
    Vector2D position_, hotspot_, size_;
    //Image *image_;
	Drawable *image_;
    Image::Mirror mirror_;
	Color color_;
	float alpha_;
    Animation *animation_;
    bool visible_, delete_image_;

};
}

#endif //HORUSEYE_FRAMEWORK_SPRITE_H_
