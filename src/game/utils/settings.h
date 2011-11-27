#ifndef HORUSEYE_GAME_UTILS_SETTINGS_H_
#define HORUSEYE_GAME_UTILS_SETTINGS_H_

#include <list>
#include <string>
#include <ugdk/math/vector2D.h>
#include <ugdk/base/types.h>

namespace utils {

typedef struct {
    char control[14];
    ugdk::uint8 resolution;
    ugdk::uint8 fullscreen;
    ugdk::uint8 background_music;
    ugdk::uint8 sound_effects;
    ugdk::uint8 language;

    void FillWithDefaultValues();
    bool ValidateData() const;
} SettingsData;

class DataSource {
  public:
    DataSource(const std::string filename) : filename_(filename) {}
    virtual ~DataSource() {}

    virtual bool Read(       SettingsData &data) const = 0;
    virtual bool Write(const SettingsData &data) const = 0;

  private:
    const std::string filename_;

  protected:
    const std::string& filename() const { return filename_; }
};

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
    void set_fullscreen(bool fullscreen)            { fullscreen_ = fullscreen; }
    void set_background_music(int background_music) { background_music_ = background_music; }
    void set_sound_effects(bool sound_effects)      { sound_effects_ = sound_effects; }
    void set_language(int language)                 { language_ = language; }

    // Getters
    int resolution()            const { return resolution_; }
    bool fullscreen()           const { return fullscreen_; }
    bool background_music()     const { return background_music_; }
    bool sound_effects()        const { return sound_effects_; }
    int language()              const { return language_; }

    const ugdk::Vector2D& resolution_vector() const;
    const std::string& language_file() const;


    const static int NUM_RESOLUTIONS = 12;
    static const ugdk::Vector2D* ResolutionList() { return resolutions_; }
    const static int NUM_LANGUAGES = 2;
    static const std::string* LanguageList() { return languages_; }
    static const std::string* LanguageNameList() { return languages_names_; }

    const std::string& root_file_path() { return root_file_path_; }

  private:
    // Singleton design.
    static Settings* reference_;
    Settings();

    static ugdk::Vector2D resolutions_[NUM_RESOLUTIONS];
    static std::string languages_[NUM_LANGUAGES], languages_names_[NUM_LANGUAGES];

    int resolution_, language_;
    bool fullscreen_, background_music_, sound_effects_;
    std::string configuration_file_path_, root_file_path_;
    std::list<DataSource*> sources_;

    void SetSettingsPath();
};
}

#endif //HORUSEYE_GAME_UTILS_SETTINGS_H_
