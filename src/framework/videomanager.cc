#include "videomanager.h"
#include "pathmanager.h"
#include "engine.h"
#include "image.h"

namespace framework {

// Inicializa o gerenciador de video, definindo uma
// resolucao para o programa. Retorna true em caso de
// sucesso.
bool VideoManager::Initialize(const string& title, const Vector2D& size,
                              bool fullscreen, const string& icon) {

    ChangeResolution(size, fullscreen);
	if(icon.length() > 0) {
		SDL_WM_SetIcon(SDL_LoadBMP(icon.c_str()), NULL);
	}
    SDL_WM_SetCaption(title.c_str(), NULL);
    title_ = title;

    blank_image_ = new Image;
    blank_image_->Create(Vector2D(200,200));
    blank_image_->set_frame_size(Vector2D(50, 50));
    blank_image_->Clear(0x00FF00);


    return true;
}

// Changes the resolution to the requested value.
// Returns true on success.
bool VideoManager::ChangeResolution(const Vector2D& size, bool fullscreen) {
    if(screen_ == NULL) screen_ = new Image;
    if(!screen_->CreateVideoSurface(size, fullscreen))
        return false;

    if(backbuffer_ != NULL) {
        backbuffer_->Destroy();
        delete backbuffer_;
    }
    backbuffer_ = new Image;
    if(backbuffer_ == NULL || !backbuffer_->Create(size)) {
        return false;
    }
    video_size_ = size;
    fullscreen_ = fullscreen;
    return true;
}

// Termina o gerenciador de video, retornando true em
// caso de sucesso.
bool VideoManager::Release() {
    map<string,Image*>::iterator it;

    for(it = memory_.begin(); it != memory_.end(); ++it) {
        Image* img = it->second;
        img->Destroy();
        delete img;
    }

    if(backbuffer_ != NULL) {
        backbuffer_->Destroy();
        delete backbuffer_;
    }

    delete screen_;

    memory_.clear();
    return true;
}

// Desenha backbuffer na tela
void VideoManager::Render() {
    backbuffer_->DrawTo(screen_, Vector2D(0, 0), 0, Image::MIRROR_NONE);
    SDL_Flip(screen_->data_);
    backbuffer_->Clear(0);
}

// Carrega imagem de um arquivo, fazendo o
// gerenciamento de memoria. Retorna NULL
// em caso de falha.
Image* VideoManager::LoadImage(const string& filepath) {
	std::string fullpath = PATH_MANAGER()->ResolvePath(filepath);
    if(memory_.count(filepath) == 0) {
        Image* img = new Image;
        if(img != NULL) {
            if(!img->LoadFromFile(fullpath)) {
                delete img;
                return NULL;
            }
            memory_[filepath] = img;
        }
        else
            return NULL;
    }

    return memory_[filepath];
}

}  // namespace framework
