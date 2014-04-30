#ifndef HORUSEYE_GAME_SCENES_CONSOLE_H_
#define HORUSEYE_GAME_SCENES_CONSOLE_H_

#include <ugdk/action/scene.h>
#include <ugdk/graphic.h>

#include <memory>
#include <vector>
#include <sstream>

namespace scene {

class Console : public ugdk::action::Scene {
  typedef ugdk::action::Scene super;
  public:
    Console();
    ~Console();

    void AddText(const std::string&);
    void RemoveAddedText();
    void ExecuteCurrentLine();

    void Focus() override;
    void DeFocus() override;

  protected:
    void UpdateCurrentLine();

    std::unique_ptr<ugdk::graphic::Node> node_;
    ugdk::graphic::TextBox* history_textbox_;
    std::list<std::string> history_;
    std::list<std::string> current_line_;
    ugdk::graphic::Label* current_line_label_;
};

} // namespace scene

#endif // HORUSEYE_GAME_SCENES_CONSOLE_H_
