#ifndef HORUSEYE_GAME_UTILS_LEVELMANAGER_H_
#define HORUSEYE_GAME_UTILS_LEVELMANAGER_H_

#include "../scenes/imagescene.h"
#include "../sprites/hero.h"

namespace scene {
class World;
class Menu;
}

namespace utils {

class LevelManager {
  public:
    static LevelManager* reference() {
		static LevelManager* r = new LevelManager();
        return r;
    }
    void Initialize();

    typedef enum {
        NOT_FINISHED,
        FINISH_WIN,
        FINISH_DIE,
        FINISH_WARP,
        FINISH_QUIT
    } LevelState;

    void ShowIntro();
    void ShowCredits();
    void ShowEnding();
    void ShowGameOver();

    void StartGame();
    void FinishLevel(LevelState);

    scene::World* get_current_level() { return current_level_; }
    void SetNextLevel(unsigned int id) { level_list_iterator_ = id; }
    unsigned int GetNextLevelID() { return level_list_iterator_; }

	void Finish();
    ~LevelManager();

    void QueueRestartGame() { restart_game_ = true; }
    bool RestartGameQueued() { return restart_game_; }

  private:
    void LoadLevelList(std::string);
    void LoadNextLevel();

    scene::World* current_level_;
    scene::Menu* menu_;
    std::vector<std::string> level_list_;
    unsigned int level_list_iterator_;
	sprite::Hero *hero_;
    LevelManager();
    bool restart_game_;
};

}

#endif /* HORUSEYE_GAME_UTILS_LEVELMANAGER_H_ */
