#ifndef HORUSEYE_FRONTEND_SCENES_CONSOLE_H_
#define HORUSEYE_FRONTEND_SCENES_CONSOLE_H_

#include <ugdk/action/scene.h>
#include <ugdk/ui.h>
#include <ugdk/text.h>
#include <ugdk/system/compatibility.h>

#include <memory>
#include <vector>
#include <sstream>

namespace frontend {
namespace scenes {

class Console : public ugdk::action::Scene {
  typedef ugdk::action::Scene super;
  public:
    Console();
    ~Console();

    static void ChangeLanguage(const std::string&);
    static bool HasConsoleActive();

    void AddText(const std::string&);
    void RemoveAddedText();
    void ExecuteCurrentLine();

    void Focus() override;
    void DeFocus() override;

  protected:
    void UpdateCurrentLine();

    std::unique_ptr<ugdk::ui::Node> node_;
    ugdk::text::TextBox* history_textbox_;
    std::list<std::string> history_;
    std::list<std::string> current_line_;
    ugdk::text::Label* current_line_label_;
};

} // namespace scenes
} // namespace frontend

#endif // HORUSEYE_FRONTEND_SCENES_CONSOLE_H_
