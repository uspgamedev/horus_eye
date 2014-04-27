#ifndef HORUSEYE_GAME_UTILS_LEVELMANAGER_H_
#define HORUSEYE_GAME_UTILS_LEVELMANAGER_H_

#include <vector>
#include <string>
#include <ugdk/action.h>

#include "game/campaigns.h"
#include "game/sprites.h"

namespace scene {
class World;
class Loading;
class Menu;
}

namespace utils {

class LevelManager {
  public:
    static LevelManager* reference();
    ~LevelManager();

    void InitializeCampaign(const campaigns::CampaignDescriptor&);

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

    scene::World* current_level() const { return current_level_; }
    void SetNextLevel(unsigned int id) { level_list_iterator_ = id; }
    unsigned int GetNextLevelID() const { return level_list_iterator_; }

    void LoadNextLevel();
    void Finish();

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
};

}

#endif /* HORUSEYE_GAME_UTILS_LEVELMANAGER_H_ */
