//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/videomanager.h
// Definicao da classe VideoManager.
//

#ifndef HORUSEYE_FRAMEWORK_VIDEOMANAGER_H_
#define HORUSEYE_FRAMEWORK_VIDEOMANAGER_H_

#include <string>
#include <map>
#include "vector2D.h"
#include "image.h"
using std::string;
using std::map;

namespace framework {

// Gerenciador de video
class VideoManager {
  public:
    static const int COLOR_DEPTH = 32;

    VideoManager() : backbuffer_(NULL), fullscreen_(false) {}
    ~VideoManager() {}

    bool Initialize(const Vector2D& size, bool fullscreen);
    bool Release();
    void Render();

    Image* LoadImage(const string& filepath);

    Vector2D video_size() const { return video_size_; }
    bool fullscreen() const { return fullscreen_; }
    Image* backbuffer() const { return backbuffer_; }

  private:
    Image* backbuffer_;
    Image screen_;
    Vector2D video_size_;
    bool fullscreen_;
    map<string, Image*> memory_;
};

}  // namespace framework

#endif
