#ifndef UGDK_GRAPHIC_VIDEOMANAGER_H_
#define UGDK_GRAPHIC_VIDEOMANAGER_H_

#include <string>
#include <map>
#include <list>
#include <stack>
#include <ugdk/base/types.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/math/frame.h>
#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/modifier.h>

using std::string;
using std::map;

#define VIDEO_MANAGER() (ugdk::Engine::reference()->video_manager())

namespace ugdk {
namespace graphic {

class VideoManager {
  public:
    static const int COLOR_DEPTH = 32;

    VideoManager() : settings_(false, false, false), light_buffer_(NULL), light_texture_(NULL) {}
    ~VideoManager() {}

    bool Initialize(const std::string& title, const Vector2D& size, bool fullscreen, const std::string& icon);
    bool ChangeResolution(const Vector2D& size, bool fullscreen);
    bool Release();
    
    void SetVSync(const bool active);
    void SetLightSystem(const bool active) { settings_.light_system = active; }
    
    void Render(std::list<Scene*>&, std::list<Node*>&, float dt);

    Texture* LoadTexture(const std::string& filepath);

    Spritesheet* GetSpritesheet(const std::string& tag) { return spritesheet_memory_[tag]; }
    void AddSpritesheet(const std::string& tag, Spritesheet* spritesheet) {
        spritesheet_memory_[tag] = spritesheet;
    }

    Vector2D video_size() const { return video_size_; }
    bool fullscreen() const { return settings_.fullscreen; }

    const std::string& title() const { return title_; }
    const Texture* light_texture() const { return light_texture_; }

    Frame virtual_bounds() const { return virtual_bounds_; }

    void PushAndApplyModifier(const Modifier*);
    void PushAndApplyModifier(const Modifier& apply) { PushAndApplyModifier(&apply); }
    bool PopModifier();
    const Modifier& CurrentModifier() const {
        return (modifiers_.empty()) ? Modifier::IDENTITY : modifiers_.top(); 
    }

  private:
    Vector2D video_size_;
    Frame virtual_bounds_;
    std::string title_;

    struct Settings {
        bool fullscreen;
        bool vsync;
        bool light_system;

        Settings(bool fs, bool vs, bool light) : fullscreen(fs), vsync(vs), light_system(light) {}
    } settings_;

    std::map<std::string, Texture*> image_memory_;
    std::map<std::string, Spritesheet*> spritesheet_memory_;
    std::stack<Modifier> modifiers_;

    Texture* light_buffer_;
    Texture* light_texture_;

    void InitializeLight();

    void MergeLights(std::list<Scene*>& scene_list);
    void BlendLightIntoBuffer();

    void ClearModiferStack();

};

}  // namespace graphic
}  // namespace ugdk

#endif
