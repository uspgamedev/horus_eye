#ifndef HORUSEYE_FRAMEWORK_FRAME_H_
#define HORUSEYE_FRAMEWORK_FRAME_H_

namespace framework {

// Representa um quadro (retangulo) na tela e/ou numa imagem.
// Util para trabalhar com animacoes, detectar colisoes
// (bounding box), etc.
//
// left, top, right e bottom sao usados para determinar as
// coordenadas do retangulo.
//
//   (left,top)
//        +------------------+
//        |                  |
//        |                  |
//        +------------------+
//                        (right,bottom)
class Frame {
  public:
    // Construtores e destrutores
    Frame() : left_(0), top_(0), right_(0), bottom_(0) { }
    Frame(int left, int top, int right, int bottom) :
        left_(left), top_(top), right_(right), bottom_(bottom) { }
    ~Frame() { }

    // Utilidades inline
    int width() const { return right_ - left_; }
    int height() const { return bottom_ - top_; }

    // Accessors e mutators
    int left() const { return left_; }
    void set_left(int left) { left_ = left; }
    int top() const { return top_; }
    void set_top(int top) { top_ = top; }
    int right() const { return right_; }
    void set_right(int right) { right_ = right; }
    int bottom() const { return bottom_; }
    void set_bottom(int bottom) {bottom_ = bottom; }

    // Devolve true se houver colisao entre
    // 'frame' e este retangulo.
    bool Collides(const Frame& frame) const;

  private:
    int left_, top_, right_, bottom_;
};

}  // namespace framework

#endif  // HORUSEYE_FRAMEWORK_FRAME_H_
