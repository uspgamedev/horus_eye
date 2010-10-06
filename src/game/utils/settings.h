#ifndef HORUSEYE_GAME_UTILS_SETTINGS_H_
#define HORUSEYE_GAME_UTILS_SETTINGS_H_

#include <string>
#include "../../framework/vector2D.h"
#include "../../framework/types.h"
namespace utils {

class Settings {
  public:
	Settings(std::string);
	~Settings();
	void WriteToDisk();
	void set_resolution(int resolution);
	framework::Vector2D resolution();
	void set_fullscreen(bool fullscreen);
    bool fullscreen();
    int resolution_int();

  private:
    int resolution_;
	bool fullscreen_;
	static framework::Vector2D resolutions_[12];
	typedef struct data_{
	    framework::uint8 resolution;
	    framework::uint8 fullscreen;
	} Data;

	Data data_;
};
}

#endif //HORUSEYE_GAME_UTILS_SETTINGS_H_
