#include <algorithm>
#include <ugdk/portable/tr1.h>
#include FROM_TR1(functional)
#include <locale>
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>

#include <sstream>
#include <externals/inifile.h>

#include "settings.h"

#include "game/utils/constants.h"

#ifdef WIN32
#include <windows.h>
#include <shlobj.h>
#include <direct.h>
#pragma comment(lib, "shell32.lib")
#endif


/* Util functions found at http://stackoverflow.com/q/217605 */
// Requires <algorithm>, <functional>, <locale>, <cctype> and <string> in this order.
// trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}
// trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}
// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

namespace utils {

static inline std::string &tolower(std::string &str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

static bool isStringTrue(std::string& str) {
    return std::string("true").compare(tolower(trim(str))) == 0;
}
static bool isStringFalse(std::string& str) {
    return std::string("false").compare(tolower(trim(str))) == 0;
}
static inline bool StringToBool(std::string& s, bool unknown = true) {
    if(unknown)
        return !isStringFalse(s);
    else
        return isStringTrue(s);
}
static inline std::string BoolToString(const bool& value) {
    return value ? "true" : "false";
}
static inline std::string IntToString(const int& x) {
    std::ostringstream o;
    if (!(o << x))
        return "";
    return o.str();
}
static inline int StringToInt(const std::string& s) {
    std::istringstream i(s);
    int x;
    if (!(i >> x))
        return -1;
    return x;
}


void SettingsData::FillWithDefaultValues() {
    resolution = 1;
    fullscreen = false;
    background_music = true;
    sound_effects = true;
    language = 0;
}

bool SettingsData::ValidateData() const {
    if((strncmp(control, "HORUSCONFIGV", 12) != 0) || (control[12] - '0' < 1)) {
        // Invalid file or invalid version
        return false;
    }
    return true;
}

class IniFileSource : public DataSource {
  public:
    IniFileSource(const std::string& filepath)
        : DataSource(filepath + Constants::INI_CONFIGURATION_FILENAME) {}

    virtual bool Read(SettingsData &data) const {
        externals::CIniFile source;
        if(!source.Load(filename())) return false;

        externals::CIniSection* section = source.GetSection("Settings");
        if(section == NULL) return false;

        std::string fullscreen =    section->GetKeyValue("Fullscreen");
        std::string soundeffect =   section->GetKeyValue("SoundEffects");
        std::string music =         section->GetKeyValue("Music");
        std::string language =      section->GetKeyValue("Language");
        std::string resolutionx =   section->GetKeyValue("ResolutionX");
        std::string resolutiony =   section->GetKeyValue("ResolutionY");

        data.FillWithDefaultValues();

        strcpy(data.control, "HORUSCONFIGV1");
        data.fullscreen =       StringToBool(fullscreen, false);
        data.sound_effects =    StringToBool(soundeffect, true);
        data.background_music = StringToBool(music, true);

        tolower(trim(language));
        const std::string* languages = Settings::LanguageNameList();
        for(int i = 0; i < Settings::NUM_LANGUAGES; ++i) {
            std::string name = std::string(languages[i]); // copy
            tolower(trim(name));
            if(name.compare(language) == 0) {
                data.language = i;
                break;
            }
        }

        ugdk::Vector2D resolution((double) StringToInt(resolutionx), (double) StringToInt(resolutiony));
        const ugdk::Vector2D* resolution_list = Settings::ResolutionList();
        for(int i = 0; i < Settings::NUM_RESOLUTIONS; ++i) {
            if(resolution.x == resolution_list[i].x && resolution.y == resolution_list[i].y) {
                data.resolution = i;
                break;
            }
        }

        return true;
    }

    virtual bool Write(const SettingsData &data) const {
        externals::CIniFile destination;
        externals::CIniSection* sect = destination.AddSection("Settings");
        sect->AddKey("Fullscreen")->SetValue(   BoolToString(data.fullscreen));
        sect->AddKey("SoundEffects")->SetValue( BoolToString(data.sound_effects));
        sect->AddKey("Music")->SetValue(        BoolToString(data.background_music));
        sect->AddKey("Language")->SetValue(     Settings::LanguageNameList()[data.language]);
        sect->AddKey("ResolutionX")->SetValue(  IntToString((int)(Settings::ResolutionList()[data.resolution].x)));
        sect->AddKey("ResolutionY")->SetValue(  IntToString((int)(Settings::ResolutionList()[data.resolution].y)));
        return destination.Save(filename());
    }
};

class BinaryFileSource : public DataSource {
  public:
    BinaryFileSource(const std::string& filepath)
        : DataSource(filepath + Constants::BINARY_CONFIGURATION_FILENAME) {}

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
        Vector2D(800.0,600.0),
        Vector2D(1024.0,768.0),
        Vector2D(1280.0,720.0),
        Vector2D(1280.0,800.0),
        Vector2D(1280.0,960.0),
        Vector2D(1280.0,1024.0),
        Vector2D(1366.0,768.0),
        Vector2D(1440.0,900.0),
        Vector2D(1600.0,900.0),
        Vector2D(1600.0,1080.0),
        Vector2D(1920.0,1080.0),
        Vector2D(1920.0,1200.0)
};

std::string Settings::languages_[] = {
        "en_US",
        "pt_BR"
};

std::string Settings::languages_names_[] = {
        "English", "Portugues"
};

Settings::Settings() {
    SetSettingsPath();

    this->sources_.push_back(new IniFileSource(   configuration_folder_path_));
    this->sources_.push_back(new BinaryFileSource(configuration_folder_path_));
    this->sources_.push_back(new IniFileSource(   ""));
    this->sources_.push_back(new BinaryFileSource(""));

    SettingsData data;
    if(!ReadFromDisk(data))
        data.FillWithDefaultValues();

    resolution_ = data.resolution;
    fullscreen_ = data.fullscreen;
    background_music_ = data.background_music;
    sound_effects_ = data.sound_effects;
    language_ = data.language;
}

bool Settings::ReadFromDisk(SettingsData &data) {
    std::list<DataSource*>::iterator it;
    for(it = sources_.begin(); it != sources_.end(); ++it) {
        if((*it)->Read(data) && data.ValidateData())
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

const std::string& Settings::language_name() const {
    return languages_[language_];
}

void Settings::SetSettingsPath() {
#ifdef WIN32
    CHAR my_documents[MAX_PATH];
    HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, my_documents);
    configuration_folder_path_ = std::string(my_documents) + "/Horus Eye/";
    if(GetFileAttributes(configuration_folder_path_.c_str()) == INVALID_FILE_ATTRIBUTES)
        _mkdir(configuration_folder_path_.c_str());
#else
    char* home = getenv("HOME");
    if(home) configuration_folder_path_ = std::string(home) + "/.config/horus_eye/";
    else configuration_folder_path_ = "";
#endif
}

}
