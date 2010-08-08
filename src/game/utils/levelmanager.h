/*
 * levelmanager.h
 *
 *  Created on: Aug 4, 2010
 *      Author: Henrique
 */

#ifndef HORUSEYE_GAME_UTILS_LEVELMANAGER_H_
#define HORUSEYE_GAME_UTILS_LEVELMANAGER_H_

#include "../scenes/imagescene.h"

namespace scene {
class World;
class Menu;
}

namespace utils {

class LevelManager {
  public:
    static LevelManager* reference() {
        static LevelManager *r = NULL;
        return r ? r : r = new LevelManager;
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
    void ShowEnding();
    void ShowGameOver();

    void StartGame(scene::ImageScene::SceneType type);
    void FinishLevel(LevelState);

    scene::World* get_current_level() { return current_level_; }
    void SetNextLevel(unsigned int id) { level_list_iterator_ = id; }

    ~LevelManager();

  private:
    void LoadNextLevel();

    scene::World* current_level_;
    scene::Menu* menu_;
    std::vector<std::string> level_list_;
    unsigned int level_list_iterator_;

    LevelManager();
};

}

#endif /* HORUSEYE_GAME_UTILS_LEVELMANAGER_H_ */
