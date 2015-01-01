#include "console.h"

#include <ugdk/system/engine.h>
#include <ugdk/action.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/graphic/module.h>
#include <ugdk/ui/node.h>
#include <ugdk/ui/drawable.h>
#include <ugdk/ui/drawable/texturedrectangle.h>
#include <ugdk/text/module.h>
#include <ugdk/text/label.h>
#include <ugdk/text/textbox.h>
#include <ugdk/input/events.h>
#include <ugdk/input/module.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/langwrapper.h>
#include <ugdk/system/compatibility.h>

#include <functional>

namespace frontend {
namespace scenes {

using namespace ugdk;
using namespace ugdk::ui;
using namespace std::placeholders;

namespace {
    std::size_t MAX_HISTORY_SIZE = 10;
    std::string CONSOLE_LANGUAGE = "Lua";
    bool g_has_console_active_ = false;
}

Console::Console()
: node_(new Node) {
    double console_height = 300.0;
    text::Font* font = ugdk::text::manager()->GetFont("DejaVuMono");

    auto gmngr = graphic::manager();
    std::shared_ptr<Node> bg(new Node(ugdk::MakeUnique<TexturedRectangle>(gmngr->white_texture(), math::Vector2D(gmngr->screen()->size().x, console_height))));
    bg->effect().set_color(Color(0x000000, 1.0));
    node_->AddChild(bg);

    current_line_label_ = new text::Label("> ", font);
    current_line_label_->set_hotspot(HookPoint::BOTTOM_LEFT);

    std::shared_ptr<Node> current_line_node(new Node(std::unique_ptr<Drawable>(current_line_label_)));
    current_line_node->geometry().set_offset(math::Vector2D(0, console_height - 5.0));
    node_->AddChild(current_line_node);
    
    history_textbox_ = new text::TextBox("", gmngr->screen()->size().x, font);
    
    std::shared_ptr<Node> history_node(new Node(std::unique_ptr<Drawable>(history_textbox_)));
    history_node->geometry().set_offset(current_line_node->geometry().offset());
    node_->AddChild(history_node);

    event_handler().AddListener<input::TextInputEvent>([this](const input::TextInputEvent& ev) {
        this->AddText(ev.text);
    });
    event_handler().AddListener<input::KeyPressedEvent>([this](const input::KeyPressedEvent& ev) {
        if (ev.scancode == ugdk::input::Scancode::RETURN)
            this->ExecuteCurrentLine();
        else if (ev.scancode == ugdk::input::Scancode::BACKSPACE)
            this->RemoveAddedText();
        else if (ev.scancode == ugdk::input::Scancode::ESCAPE)
            this->Finish();
    });
    
    set_render_function(std::bind(&Node::Render, node_.get(), _1));
    g_has_console_active_ = true;
}

Console::~Console() {
    g_has_console_active_ = false;
}
    
void Console::ChangeLanguage(const std::string& lang) {
    if (SCRIPT_MANAGER()->GetWrapper(lang)) {
        CONSOLE_LANGUAGE = lang;
    }
}

bool Console::HasConsoleActive() {
    return g_has_console_active_;
}

void Console::AddText(const std::string& line) {
    current_line_.push_back(line);
    UpdateCurrentLine();
}
    
void Console::RemoveAddedText() {
    if (!current_line_.empty()) {
        current_line_.pop_back();
        UpdateCurrentLine();
    }
}

void Console::ExecuteCurrentLine() {
    std::stringstream line_to_execute;
    for (const auto& part : current_line_)
        line_to_execute << part;
    history_.push_back("> " + line_to_execute.str());

    SCRIPT_MANAGER()->GetWrapper(CONSOLE_LANGUAGE)->ExecuteCode(line_to_execute.str());

    while (history_.size() > MAX_HISTORY_SIZE)
        history_.pop_front();

    std::stringstream ss;
    for (const auto& line : history_)
        ss << line << "\n";
    history_textbox_->ChangeMessage(ss.str());
    history_textbox_->set_hotspot(ugdk::ui::HookPoint::BOTTOM_LEFT);

    current_line_.clear();
    current_line_label_->ChangeMessage("> ");
}

void Console::Focus() {
    Scene::Focus();
    input::manager()->text_input().Start(math::Integer2D(0, 200), math::Integer2D(400, 100));
}

void Console::DeFocus() {
    Scene::DeFocus();
    input::manager()->text_input().Stop();
}
    
void Console::UpdateCurrentLine() {
    std::stringstream ss;
    ss << "> ";
    for (const auto& part : current_line_)
        ss << part;
    current_line_label_->ChangeMessage(ss.str());
}

} // namespace scenes
} // namespace frontend

