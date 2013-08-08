#ifndef HORUSEYE_GAME_UTILS_LEVELMANAGER_H_
#define HORUSEYE_GAME_UTILS_LEVELMANAGER_H_

#include <vector>
#include <string>
#include <ugdk/action.h>
#include "game/sprites.h"

namespace scene {
class World;
class Loading;
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

    void FinishLevel(LevelState);

    const std::string& current_campaign() const { return current_campaign_; }

    scene::World* current_level() { return current_level_; }
    void SetNextLevel(unsigned int id) { level_list_iterator_ = id; }
    unsigned int GetNextLevelID() { return level_list_iterator_; }

    void LoadNextLevel();
    void Finish();
    ~LevelManager();

    void QueueRestartGame() { restart_game_ = true; }
    bool RestartGameQueued() { return restart_game_; }

    void ChangeCampaign(const std::string& name, const std::vector<std::string>& list) { 
        current_campaign_ = name;
        level_list_ = list;
    }

    void InformLoadingDeleted() { loading_ = NULL; }

#ifdef DEBUG
    void DebugLoadSpecificLevel(const std::string& level_name) {
        loadSpecificLevel(level_name);
    }
#endif

  private:
    LevelManager();
    void loadSpecificLevel(const std::string& level_name);

    scene::World* current_level_;
    ugdk::action::Scene* menu_;
    scene::Loading* loading_;
    std::string current_campaign_;
    std::vector<std::string> level_list_;
    unsigned int level_list_iterator_;
    bool restart_game_;
};

}

#endif /* HORUSEYE_GAME_UTILS_LEVELMANAGER_H_ */
