#include <string>
#include <sstream>
//#include <externals/inifile.h>

#include "settings.h"

#include "game/config.h"
#include "game/utils/constants.h"

#ifdef WIN32
#include <windows.h>
#include <shlobj.h>
#include <direct.h>
#pragma comment(lib, "shell32.lib")
#endif

namespace utils {


class IniFileSource : public DataSource {
  public:
    IniFileSource(const std::string& filepath)
        : DataSource(filepath + "settings.ini") {}

    virtual bool Read(SettingsData &data) const {
        /*CIniFile source;
        if(!source.Load(filename())) return false;

        CIniSection* section = source.GetSection("Settings");
        if(section == NULL) return false;

        //section->GetKeyValue(*/

        return false;
    }

    virtual bool Write(const SettingsData &data) const {
        return false;
    }
};

class BinaryFileSource : public DataSource {
  public:
    BinaryFileSource(const std::string& filepath)
        : DataSource(filepath + "settings.bin") {}

    virtual bool Read(SettingsData &data) const {
        FILE *source = fopen(filename().c_str(),"rb");
        if(source == NULL) return false;

        // Reading the data from the file
        fread(&data, sizeof(SettingsData), 1, source);

        // Cleanup.
        fclose(source);
        return true;
    }

    virtual bool Write(const SettingsData &data) const {
        // Opening the file.
        FILE *destination = fopen(filename().c_str(), "wb");
        if(destination == NULL) return false;

        // Writing the data.
        fwrite(&data, sizeof(SettingsData), 1, destination);

        fclose(destination);
        return true;
    }
};


Settings* Settings::reference_ = NULL;

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
    SetSettingsPath();

    this->sources_.push_back(new IniFileSource(   configuration_file_path_));
    this->sources_.push_back(new BinaryFileSource(configuration_file_path_));
    this->sources_.push_back(new IniFileSource(   ""));
    this->sources_.push_back(new BinaryFileSource(""));

    SettingsData data;
    if(!ReadFromDisk(data))
        FillWithDefaultValues(data);

    resolution_ = data.resolution;
    fullscreen_ = data.fullscreen;
    background_music_ = data.background_music;
    sound_effects_ = data.sound_effects;
    language_ = data.language;
}

bool Settings::ReadFromDisk(SettingsData &data) {
    std::list<DataSource*>::iterator it;
    for(it = sources_.begin(); it != sources_.end(); ++it) {
        if((*it)->Read(data) && ValidateData(data))
            return true;
    }
    return false;
}

bool Settings::WriteToDisk() {
    SettingsData data;
    strcpy(data.control, "HORUSCONFIGV1");
    data.resolution = resolution_;
    data.fullscreen = fullscreen_;
    data.background_music = background_music_;
    data.sound_effects = sound_effects_;
    data.language = language_;

    std::list<DataSource*>::iterator it;
    for(it = sources_.begin(); it != sources_.end(); ++it) {
        if((*it)->Write(data))
            return true;
    }
    return false;
}

const Vector2D& Settings::resolution_vector() const {
    return resolutions_[resolution_];
}

const std::string& Settings::language_file() const {
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

    configuration_file_path_ = stm.str();
}


void Settings::FillWithDefaultValues(SettingsData &data) const {
    data.resolution = 1;
    data.fullscreen = false;
    data.background_music = true;
    data.sound_effects = true;
    data.language = 0;
}

bool Settings::ValidateData(const SettingsData &data) const {
    if((strncmp(data.control, "HORUSCONFIGV", 12) != 0) || (data.control[12] - '0' < 1)) {
        // Invalid file or invalid version
        return false;
    }
    return true;
}

}
