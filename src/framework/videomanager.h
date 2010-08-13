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

#define VIDEO_MANAGER() framework::Engine::reference()->video_manager()

namespace framework {

// Gerenciador de video
class VideoManager {
  public:
    static const int COLOR_DEPTH = 32;

    VideoManager() : backbuffer_(NULL), fullscreen_(false) {}
    ~VideoManager() {}

    bool Initialize(const string& title, const Vector2D& size, bool fullscreen);
    bool Release();
    void Render();

    Image* LoadImage(const string& filepath);
    Image* CreateFogTransparency(const Vector2D& size, const Vector2D& origin, const Vector2D& ellipse_coef, float radius);

    Vector2D video_size() const { return video_size_; }
    bool fullscreen() const { return fullscreen_; }
    string title() const { return title_; }
    Image* backbuffer() const { return backbuffer_; }

  private:
    Image* backbuffer_;
    Image screen_;
    Vector2D video_size_;
    bool fullscreen_;
    string title_;
    map<string, Image*> memory_;
};

}  // namespace framework

#endif
