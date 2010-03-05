//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/image.h
// Definicao da classe Image.
//

#ifndef HORUSEYE_FRAMEWORK_IMAGE_H_
#define HORUSEYE_FRAMEWORK_IMAGE_H_

#include <SDL/SDL.h>
#include <string>
#include "vector2D.h"
using std::string;

namespace framework {

// Representa uma imagem na memoria
class Image {
  friend class VideoManager;
  public:
    typedef Uint32 Color;

    typedef Uint8 Mirror;
    static const Mirror MIRROR_NONE = 0;
    static const Mirror MIRROR_HFLIP = 1;
    static const Mirror MIRROR_VFLIP = 2;

    Image() : data_(NULL) {}
    ~Image() {}

    bool Create(const Vector2D& size);
    bool Destroy();
    bool Clear(Color color);

    int width() const { return data_ ? data_->w : 0; }
    int height() const { return data_ ? data_->h : 0; }

    void set_frame_size(const Vector2D& size) { frame_size_ = size; }
    Vector2D frame_size() const { return frame_size_; }
    int FrameCount() const;

    bool DrawTo(Image* dest, const Vector2D& position, int frame_number,
                Mirror mirror);



  private:
    SDL_Surface *data_;
    Vector2D frame_size_;

    // apenas o VideoManager acessa
    bool LoadFromFile(const string& file);

    // apenas o VideoManager acessa
    bool CreateVideoSurface(const Vector2D& size, bool fullscreen);
};

}  // namespace framework

#endif
