#ifndef HORUSEYE_GAME_UTILS_SETTINGS_H_
#define HORUSEYE_GAME_UTILS_SETTINGS_H_

#include <list>
#include <string>
#include <ugdk/math/integer2D.h>
#include <ugdk/structure/types.h>

namespace utils {

typedef struct {
    char control[14];
    ugdk::uint8 resolution;
    ugdk::uint8 fullscreen;
    ugdk::uint8 background_music;
    ugdk::uint8 sound_effects;
    ugdk::uint8 language;
    ugdk::uint8 vsync;

    void FillWithDefaultValues();
    bool ValidateData() const;
} SettingsData;

class DataSource;

class Settings {
  public:
    static Settings* reference() {
        return reference_ ? reference_ : (reference_ = new Settings());
    }

    ~Settings() {}

    bool ReadFromDisk(SettingsData &data);
    bool WriteToDisk();

    // Setters
    void set_resolution(int resolution)             { resolution_ = resolution; }
    void set_fullscreen(int fullscreen)             { fullscreen_ = fullscreen != 0; }
    void set_background_music(int background_music) { background_music_ = background_music != 0; }
    void set_sound_effects(int sound_effects)       { sound_effects_ = sound_effects != 0; }
    void set_language(int language)                 { language_ = language; }
    void set_vsync(int vsync)                        { vsync_ = vsync != 0;  }

    // Getters
    int resolution()            const { return resolution_; }
    bool fullscreen()           const { return fullscreen_; }
    bool background_music()     const { return background_music_; }
    bool sound_effects()        const { return sound_effects_; }
    int language()              const { return language_; }
    bool vsync()                const { return vsync_; }

    const ugdk::math::Integer2D& resolution_vector() const;
    const std::string& language_name() const;

    const static int NUM_RESOLUTIONS = 12;
    static const ugdk::math::Integer2D* ResolutionList() { return resolutions_; }
    const static int NUM_LANGUAGES = 2;
    static const std::string* LanguageList() { return languages_; }
    static const std::string* LanguageNameList() { return languages_names_; }

  private:
    // Singleton design.
    static Settings* reference_;
    Settings();

    static ugdk::math::Integer2D resolutions_[NUM_RESOLUTIONS];
    static std::string languages_[NUM_LANGUAGES], languages_names_[NUM_LANGUAGES];

    int resolution_, language_;
    bool fullscreen_, background_music_, sound_effects_, vsync_;
    std::string configuration_folder_path_;
    std::list<DataSource*> sources_;

    void SetSettingsPath();
};
}

#endif //HORUSEYE_GAME_UTILS_SETTINGS_H_
