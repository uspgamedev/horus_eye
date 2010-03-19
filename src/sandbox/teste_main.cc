// um Main de testes...
// sandbox
#include <cstdio>
#include <iostream>
#include "../framework/videomanager.h"
#include "../framework/vector2D.h"
#include "../framework/image.h"
#include "../framework/inputmanager.h"
using namespace framework; // nao respeita coding style
using namespace std;


class Main {
  private:
    bool gameover_;
    VideoManager video_;
    InputManager input_;
    Main() : gameover_(false) {}

  public:
    static Main* reference() { static Main *r = NULL; return r ? r : r=new Main; }
    VideoManager* video() { return &video_; }

    bool Initialize() {
        SDL_Init(SDL_INIT_EVERYTHING);
        video()->Initialize("ola mundo", Vector2D(640,480), false);
        return true;
    }

    bool Release() {
        video()->Release();
        SDL_Quit();
        return true;
    }

    void Run() {
        //while(!gameover_) {
        //}
        Image *image = video()->LoadImage("SDL_logo.png");
        image->DrawTo(video()->backbuffer(), Vector2D(100,100), 0, Image::MIRROR_NONE);
        video()->Render();
        while(!input_.KeyPressed(SDLK_0)) {
            input_.Update(0.2);
        }
    }
};


int main(int argc, char* argv[])
{
    Main *main = Main::reference();
    main->Initialize();
    main->Run();
    main->Release();

    return 0;
}
