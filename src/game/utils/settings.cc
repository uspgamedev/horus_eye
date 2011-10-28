#include <string>
#include <sstream>
#include "../config.h"
#include "settings.h"
#include "constants.h"

#ifdef WIN32
#include <windows.h>
#include <shlobj.h>
#include <direct.h>
#pragma comment(lib, "shell32.lib")
#endif

namespace utils {

using namespace ugdk;

Vector2D Settings::resolutions_[] = {
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

std::string Settings::languages_[] = {
        "data/text/lang_en.txt",
        "data/text/lang_pt_br.txt"
};

std::string Settings::languages_names_[] = {
        "English", "Portugues"
};

Settings::Settings() {
    Data data;
	SetSettingsPath(); 
	FILE *settings = fopen(configuration_file_path_.c_str(),"rb");
    if(settings == NULL) {
        settings = fopen(Constants::CONFIGURATION_FILE.c_str(), "rb");
    }
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
	FILE *settings = fopen(configuration_file_path_.c_str(),"wb");
	if(settings == NULL) {
	    settings = fopen(Constants::CONFIGURATION_FILE.c_str(), "wb");
	}
	if(settings == NULL) {
#ifdef DEBUG
	    printf("Couldn't open file: %s\n", configuration_file_path_.c_str());
#endif
	    return;
	}
	strcpy(data.control, "HORUSCONFIGV1");
	data.resolution = resolution_;
	data.fullscreen = fullscreen_;
	data.background_music = background_music_;
	data.sound_effects = sound_effects_;
	data.language = language_;
	fwrite(&data, sizeof(Data), 1, settings);
	fclose(settings);
}

const Vector2D& Settings::resolution_vector(){
    return resolutions_[resolution_];
}

const std::string& Settings::language_file() {
    return languages_[language_];
}

void Settings::SetSettingsPath() {
    std::ostringstream stm;

#ifdef WIN32
    CHAR my_documents[MAX_PATH];
    HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, my_documents);
	stm << my_documents << "/Horus Eye/";
	if(GetFileAttributes(stm.str().c_str()) == INVALID_FILE_ATTRIBUTES)
		mkdir(stm.str().c_str());
#else
	stm << USER_HOME << "/.horus_eye/";
#endif

	std::ostringstream path_to_path_file;
	path_to_path_file << stm.str() << Constants::ROOT_PATH_FILE;
	root_file_path_ = path_to_path_file.str();

	stm << Constants::CONFIGURATION_FILE;
	configuration_file_path_ = stm.str();
}

}
