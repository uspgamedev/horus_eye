#ifndef HORUSEYE_FRAMEWORK_VIDEOMANAGER_H_
#define HORUSEYE_FRAMEWORK_VIDEOMANAGER_H_

#include <string>
#include <map>
#include "vector2D.h"
using std::string;
using std::map;

#define VIDEO_MANAGER() framework::Engine::reference()->video_manager()

namespace framework {
class Image;
class Texture;

// Gerenciador de video
class VideoManager {
  public:
    static const int COLOR_DEPTH = 32;

    VideoManager() : fullscreen_(false) {}
    ~VideoManager() {}

    bool Initialize(const string& title, const Vector2D& size, bool fullscreen);
    bool Release();
    void Render();

    Texture* LoadTextureFromFile(const string& filepath);
    Image* LoadImageFile(const string& filepath);
    Image* LoadImage(const string& filepath) {
        return LoadImageFile(filepath);
    }

    Vector2D video_size() const { return video_size_; }
    bool fullscreen() const { return fullscreen_; }
    string title() const { return title_; }
	Image* backbuffer() const { return NULL; }

  private:
    Vector2D video_size_;
    bool fullscreen_;
    string title_;
    map<string, Texture*> memory_;
    map<string, Image*> image_memory_;
};

}  // namespace framework

#endif
