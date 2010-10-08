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
    Data data;
    FILE *settings = fopen(filename.c_str(),"rb");
    if(settings != NULL) {
        fread(&data, sizeof(Data), 1, settings);
        fclose(settings);
        if((strncmp(data.control, "HORUSCONFIGV", 12) != 0)
                || (data.control[12] - '0' < 1)) {
            // Invalid file or invalid version
            settings = NULL;
        }
    }
    // if settings is NULL, use default values.
    if (settings==NULL)
    {
        resolution_ = 1;
        fullscreen_ = false;
        background_music_ = true;
        sound_effects_ = true;
        language_ = 0;

        WriteToDisk();
    } else {
        resolution_ = data.resolution;
        fullscreen_ = data.fullscreen;
        background_music_ = data.background_music;
        sound_effects_ = data.sound_effects;
        language_ = data.language;
    }
}

Settings::~Settings(){
}

void Settings::WriteToDisk(){
    Data data;
	FILE *settings = fopen("settings.duh","wb");
	strcpy(data.control, "HORUSCONFIGV1");
	data.resolution = resolution_;
	data.fullscreen = fullscreen_;
	data.background_music = background_music_;
	data.sound_effects = sound_effects_;
	data.language = language_;
	fwrite(&data, sizeof(Data), 1, settings);
	fclose(settings);
}

Vector2D Settings::resolution_vector(){
    return resolutions_[resolution_];
}

}
