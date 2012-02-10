#ifndef UGDK_BASE_ENGINE_H_
#define UGDK_BASE_ENGINE_H_

#include <vector>
#include <list>
#include <string>
#include <ugdk/action.h>
#include <ugdk/audio.h>
#include <ugdk/graphic.h>
#include <ugdk/input.h>
#include <ugdk/time.h>
#include <ugdk/base/types.h>
#include <ugdk/math/vector2D.h>

namespace ugdk {
namespace base {
class ResourceManager;
}

class PathManager;

/// The game engine. Manages scenes, time, and the Audio, Video, Input and Text managers.
/** Engine is a singleton. Use Engine::reference to get a pointer to an engine.
 * @see reference()
 */
class Engine {
  public:
    /// Returns a pointer to the current Engine. Creates an Engine if there isn't one.
    static Engine* reference() { return reference_ ? reference_ : reference_ = new Engine; }

    /// Returns a reference to the Audio Manager.
    /** @see AudioManager
     */
    AudioManager *audio_manager() { return audio_manager_; }

    /// Returns a reference to the Video Manager.
    /** @see VideoManager
     */
    graphic::VideoManager *video_manager() { return video_manager_; }

    /// Returns a reference to the Text Manager.
    /** @see TextManager
     */
    graphic::TextManager *text_manager() { return text_manager_; }

    /// Returns a reference to the Input Manager.
    /** @see InputManager
     */
    input::InputManager *input_manager() { return input_manager_; }
    /// Returns a reference to the Time Handler.
    /** @see TimeManager
     */
    time::TimeManager *time_handler() { return time_manager_; }
    /// Returns a reference to the Path Manager.
    /** @see PathManager
     */
    PathManager *path_manager() { return path_manager_; }
    /// Returns a reference to the Resource Manager.
    /** @see ResourceManager
     */
    base::ResourceManager *resource_manager() { return resource_manager_; }

    /// Returns the window dimensions.
    Vector2D window_size();

    /**@name Engine Management
     * @{
     */
    /// Initializes the engine. Returns true if sucessful, false otherwise.
    /** @param windowTitle The window title.
     * @param windowSize The window's dimensions.
     */
	bool Initialize(std::string windowTitle, Vector2D windowSize, 
		bool fullscreen,
		std::string base_path = std::string("./"),
		std::string icon = std::string());
    /// Starts running the engine.
    void Run();
    /// Stops and frees the engine.
    void Release();

    /** @}
     */

    /** @name Scene Management
     * @{
     */
    // Gerenciamento de cenas

    /// Puts a scene onto the scene list, on top.
    /** @param scene The scene to be put.
     */
    void PushScene(Scene* scene);
    /// Returns the current scene.
    Scene* CurrentScene() const;
    /// Removes the top scene.
    void PopScene();

    // TODO: SERIOUSLY FIX THIS. Big fat ugly code to allow some special 
    // layers to be unnafected by the light system. Meant for user interface.
    /// Pushes a layer unaffected by the light system. Meant for interfaces.
    void PushInterface(graphic::Node* node);
    /// Removes a layer pushed by the above function.
    void RemoveInterface(graphic::Node* node);
    /** @}
     */
    /// Returns the current running FPS.
    uint32 current_fps() { return reported_fps_; }

    // Saida do motor
    /// Stops the engine and clears the scene list.
    void quit() { quit_ = true; }

    ~Engine() { reference_ = NULL; }

  private:
    void DeleteFinishedScenes();
    static Engine         *    reference_;

             AudioManager *   audio_manager_;
    graphic::VideoManager *   video_manager_;
    graphic:: TextManager *    text_manager_;
    input::  InputManager *   input_manager_;
    time::    TimeManager *    time_manager_;
	          PathManager *    path_manager_;
    base::ResourceManager *resource_manager_;

    bool quit_;
    std::list<Scene*> scene_list_;
    std::list<graphic::Node*> interface_list_;
    uint32 reported_fps_, frames_since_reset_, last_fps_report_;

	Engine() :
        audio_manager_(NULL),
	    video_manager_(NULL),
        text_manager_(NULL),
        input_manager_(NULL),
        time_manager_(NULL),
		path_manager_(NULL) {}
};

} // namespace ugdk

#endif

