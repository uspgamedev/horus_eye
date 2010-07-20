#include <string.h>
#include <stdarg.h>
#include "animation.h"
#include <stdlib.h>

#include <stdio.h>
namespace framework {


Animation::Animation(float fps, ...) {
    int i = 0;
    static int tmp_frame_list[256];
    va_list arg_list;

    va_start(arg_list, fps);
    while (1) {
        int next_arg = va_arg(arg_list, int);
        if (next_arg == -1) { // Indica fim da lista de argumentos
            break;
        }
        tmp_frame_list[i++] = next_arg;
    }

    n_frames_ = i;
    frames_ = (int *) malloc(n_frames_ * sizeof(int));
    memcpy(frames_, tmp_frame_list, n_frames_ * sizeof(int));

    fps_ = fps;
    elapsed_time_ = 0;
    current_frame_ = 0;
}

Animation::~Animation() {
    free(frames_);
}

void Animation::Update(float delta_t) {
    elapsed_time_ += delta_t;
    if (elapsed_time_ >= 1 / fps_) {
        current_frame_ = (current_frame_ + 1) % n_frames_;
        if (current_frame_ == 0) NotifyAllObservers();
        elapsed_time_ = 0;
    }
}

void Animation::AddObserver(Observer* observer) {
    observers.push_back(observer);
}

void Animation::NotifyAllObservers() {
    for (int i = 0; i < (int)observers.size(); ++i) {
        observers[i]->Tick();
    }
}
}





