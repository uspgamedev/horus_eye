/*
 * settings.cc
 */

#include <string>
#include "settings.h"

namespace utils {

using namespace framework;

Vector2D Settings::resolutions_[12] = {
        Vector2D(800.0f,600.0f),
        Vector2D(1024.0f,768.0f),
        Vector2D(1280.0f,720.0f),
        Vector2D(1280.0f,800.0f),
        Vector2D(1280.0f,960.0f),
        Vector2D(1280.0f,1024.0f),
        Vector2D(1366.0f,768.0f),
        Vector2D(1440.0f,900.0f),
        Vector2D(1600.0f,900.0f),
        Vector2D(1600.0f,1080.0f),
        Vector2D(1920.0f,1080.0f),
        Vector2D(1920.0f,1200.0f)
};

Settings::Settings(std::string filename) {
    FILE *settings = fopen(filename.c_str(),"rb");
    if (settings==NULL)
    {
        set_resolution(1);
        set_fullscreen(0);
    }
    else
    {
        fread(&data_, sizeof(Data), 1, settings);
        resolution_ = data_.resolution;
        fullscreen_ = data_.fullscreen;
        fclose(settings);
    }
}

Settings::~Settings(){
}

void Settings::WriteToDisk(){
	FILE *settings = fopen("settings.duh","wb");
	data_.resolution = resolution_;
	data_.fullscreen = fullscreen_;
	fwrite(&data_, sizeof(Data), 1, settings);
	fclose(settings);
}

void Settings::set_resolution(int resolution){
    resolution_ = resolution;
    WriteToDisk();
}

Vector2D Settings::resolution(){
    return resolutions_[resolution_];
}

void Settings::set_fullscreen(bool fullscreen){
    fullscreen_ = fullscreen;
    WriteToDisk();
}

bool Settings::fullscreen(){
    return fullscreen_;
}

int Settings::resolution_int(){
    return resolution_;
}

}
