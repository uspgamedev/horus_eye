
#include "settings.h"

#include "game/constants.h"

#ifdef WIN32
#include <windows.h>
#include <shlobj.h>
#include <direct.h>
#pragma comment(lib, "shell32.lib")
#endif

#include <algorithm>
#include <functional>
#include <locale>
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <sstream>
#include <externals/inifile.h>


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

namespace frontend {

namespace {

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
        if (unknown)
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


    bool ReadDataFromPath(const std::string& filepath, SettingsData &data) {
        externals::CIniFile source;
        if (!source.Load(filepath)) return false;

        externals::CIniSection* section = source.GetSection("Settings");
        if (!section) return false;

        std::string fullscreen = section->GetKeyValue("Fullscreen");
        std::string soundeffect = section->GetKeyValue("SoundEffects");
        std::string music = section->GetKeyValue("Music");
        std::string language = section->GetKeyValue("Language");
        std::string resolutionx = section->GetKeyValue("ResolutionX");
        std::string resolutiony = section->GetKeyValue("ResolutionY");
        std::string vsync = section->GetKeyValue("VSync");

        data.FillWithDefaultValues();

        data.fullscreen = StringToBool(fullscreen, false);
        data.vsync = StringToBool(vsync, true);
        data.sound_effects = StringToBool(soundeffect, true);
        data.background_music = StringToBool(music, true);

        tolower(trim(language));
        const std::string* languages = Settings::LanguageNameList();
        for (int i = 0; i < Settings::NUM_LANGUAGES; ++i) {
            std::string name = std::string(languages[i]); // copy
            tolower(trim(name));
            if (name.compare(language) == 0) {
                data.language = i;
                break;
            }
        }

        ugdk::math::Integer2D resolution(StringToInt(resolutionx), StringToInt(resolutiony));
        const ugdk::math::Integer2D* resolution_list = Settings::ResolutionList();
        for (int i = 0; i < Settings::NUM_RESOLUTIONS; ++i) {
            if (resolution.x == resolution_list[i].x && resolution.y == resolution_list[i].y) {
                data.resolution = i;
                break;
            }
        }

        return true;
    }

    bool WriteDataToPath(const std::string& filepath, const SettingsData &data) {
        externals::CIniFile destination;
        externals::CIniSection* sect = destination.AddSection("Settings");
        sect->AddKey("Fullscreen")->SetValue(BoolToString(data.fullscreen != 0));
        sect->AddKey("VSync")->SetValue(BoolToString(data.vsync != 0));
        sect->AddKey("SoundEffects")->SetValue(BoolToString(data.sound_effects != 0));
        sect->AddKey("Music")->SetValue(BoolToString(data.background_music != 0));
        sect->AddKey("Language")->SetValue(Settings::LanguageNameList()[data.language]);
        sect->AddKey("ResolutionX")->SetValue(IntToString((int)(Settings::ResolutionList()[data.resolution].x)));
        sect->AddKey("ResolutionY")->SetValue(IntToString((int)(Settings::ResolutionList()[data.resolution].y)));
        return destination.Save(filepath);
    }

}


void SettingsData::FillWithDefaultValues() {
    resolution = 2;
    fullscreen = false;
    background_music = true;
    sound_effects = true;
    language = 0;
    vsync = true;
}

Settings* Settings::reference_ = nullptr;

using ugdk::math::Integer2D;

Integer2D Settings::resolutions_[] = {
        Integer2D( 800, 600),
        Integer2D(1024, 768),
        Integer2D(1280, 720),
        Integer2D(1280, 800),
        Integer2D(1280, 960),
        Integer2D(1280,1024),
        Integer2D(1366, 768),
        Integer2D(1440, 900),
        Integer2D(1600, 900),
        Integer2D(1600,1080),
        Integer2D(1920,1080),
        Integer2D(1920,1200)
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

    this->sources_.push_back(configuration_folder_path_);
    this->sources_.push_back("");

    SettingsData data;
    if(!ReadFromDisk(data))
        data.FillWithDefaultValues();

    resolution_ = data.resolution;
    fullscreen_ = data.fullscreen;
    background_music_ = data.background_music;
    sound_effects_ = data.sound_effects;
    language_ = data.language;
    vsync_ = data.vsync;
}

bool Settings::ReadFromDisk(SettingsData &data) {
    for (const auto& path : sources_) {
        if (ReadDataFromPath(path, data))
            return true;
    }
    return false;
}

bool Settings::WriteToDisk() {
    SettingsData data;
    data.resolution = resolution_;
    data.fullscreen = fullscreen_;
    data.background_music = background_music_;
    data.sound_effects = sound_effects_;
    data.language = language_;
    data.vsync = vsync_;

    for (const auto& path : sources_) {
        if (WriteDataToPath(path, data))
            return true;
    }
    return false;
}

const Integer2D& Settings::resolution_vector() const {
    return resolutions_[resolution_];
}

const std::string& Settings::language_name() const {
    return languages_[language_];
}

void Settings::SetSettingsPath() {
#ifdef WIN32
    CHAR my_documents[MAX_PATH];
    HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, my_documents);
    if (result == S_OK) {
        configuration_folder_path_ = std::string(my_documents) + "/Horus Eye/";
        if (GetFileAttributes(configuration_folder_path_.c_str()) == INVALID_FILE_ATTRIBUTES)
            _mkdir(configuration_folder_path_.c_str());
    }
#else
    char* home = getenv("HOME");
    if(home) configuration_folder_path_ = std::string(home) + "/.config/horus_eye/";
    else configuration_folder_path_ = "";
#endif
}

} // namespace frontend
