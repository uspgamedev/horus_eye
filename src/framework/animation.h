//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/vector2D.h
// Definicao da classe Vector2D.
//
#ifndef HORUSEYE_FRAMEWORK_ANIMATION_H_
#define HORUSEYE_FRAMEWORK_ANIMATION_H_

namespace framework {

class Animation {
  public:
    Animation(float fps, ...);
    ~Animation() { }

    void set_fps(float fps) { fps_ = fps; }
    float fps() { return fps_; }

    int get_current_frame() { return frames_[current_frame_]; }
    void Update(float delta_t);

  private:
    float fps_;
    int *frames_;
    int n_frames_;
    int current_frame_;
    float elapsed_time_;

};

}
#endif


