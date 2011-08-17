#ifndef HORUSEYE_GAME_UTILS_SETTINGS_H_
#define HORUSEYE_GAME_UTILS_SETTINGS_H_

#include "../../framework/vector2D.h"
#include "../../framework/types.h"

namespace utils {

class Settings {
  public:
	Settings();
	~Settings();
	void WriteToDisk();

	// Setters
	void set_resolution(int resolution) { resolution_ = resolution; }
	void set_fullscreen(bool fullscreen) { fullscreen_ = fullscreen; }
    void set_background_music(int background_music) {
        background_music_ = background_music;
    }
    void set_sound_effects(bool sound_effects) { sound_effects_ = sound_effects; }
    void set_language(int language) { language_ = language; }

	// Getters
    int resolution() { return resolution_; }
    bool fullscreen() { return fullscreen_; }
    bool background_music() { return background_music_; }
    bool sound_effects() { return sound_effects_; }
    int language() { return language_; }
    const ugdk::Vector2D& resolution_vector();
    const std::string& language_file();


    const static int NUM_RESOLUTIONS = 12;
    static const ugdk::Vector2D* ResolutionList() { return resolutions_; }
    const static int NUM_LANGUAGES = 2;
    static const std::string* LanguageList() { return languages_; }
    static const std::string* LanguageNameList() { return languages_names_; }

	const std::string& root_file_path() { return root_file_path_; }

  private:
    int resolution_, language_;
	bool fullscreen_, background_music_, sound_effects_;
	static ugdk::Vector2D resolutions_[NUM_RESOLUTIONS];
	static std::string languages_[NUM_LANGUAGES], languages_names_[NUM_LANGUAGES];
	void SetSettingsPath();
	std::string configuration_file_path_, root_file_path_;
	typedef struct data_{
	    char control[14];
	    ugdk::uint8 resolution;
	    ugdk::uint8 fullscreen;
	    ugdk::uint8 background_music;
	    ugdk::uint8 sound_effects;
	    ugdk::uint8 language;
	} Data;
};
}

#endif //HORUSEYE_GAME_UTILS_SETTINGS_H_
