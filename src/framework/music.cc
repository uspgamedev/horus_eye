//
// Horus Eye - Framework
// Copyright (C) 2010  USPGameDev
//
// framework/music.cc
// Músicas
//

#include "music.h"

namespace framework {

Mix_Music* Music::playing_music_(NULL);

Music::Music(const std::string& filepath) : data_(NULL) {
    data_ = Mix_LoadMUS(filepath.c_str());
    Mix_HookMusicFinished(MusicDone);
}

Music::~Music() {
    if(data_) {
        Mix_FreeMusic(data_);
        data_ = NULL;
    }
}

void Music::Play() {
    Play(0);
}

void Music::PlayForever() {
    Play(-1);
}

void Music::Play(int loops) {
    if(Mix_PlayMusic(data_, loops) == 0)
        playing_music_ = data_;
}

void Music::Stop() {
    if(playing_music_ == data_) {
        Mix_HaltMusic();
        playing_music_ = NULL;
    }
}

bool Music::IsPlaying() {
    return playing_music_ == data_;
}

void Music::MusicDone() {
    playing_music_ = NULL;
}

void Music::SetVolume(float vol) {
    // TODO
}

float Music::Volume() {
    // TODO
    return 1.0f;
}

}  // namespace framework

