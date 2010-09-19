#include <SDL/SDL_opengl.h>
#include "videomanager.h"
#include "image.h"
#include "texture.h"

namespace framework {

// Inicializa o gerenciador de video, definindo uma
// resolucao para o programa. Retorna true em caso de
// sucesso.
bool VideoManager::Initialize(const string& title, const Vector2D& size,
                              bool fullscreen) {
    Uint32 flags = SDL_OPENGL;
    if(fullscreen)
        flags |= SDL_FULLSCREEN;
    if(SDL_SetVideoMode(static_cast<int>(size.x), static_cast<int>(size.y),
            VideoManager::COLOR_DEPTH, flags) == NULL)
        return false;

    glClearColor( 0, 0, 0, 0 );


    //Set projection
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, size.x, size.y, 0, -1, 1 );

    //Initialize modelview matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);            // Enable Texture Mapping
    glEnable(GL_BLEND);

    //If there was any errors
    if( glGetError() != GL_NO_ERROR )
    {
        return false;
    }

    SDL_WM_SetCaption(title.c_str(), NULL);

    video_size_ = size;
    fullscreen_ = fullscreen;
    title_ = title;

    return true;
}

// Termina o gerenciador de video, retornando true em
// caso de sucesso.
bool VideoManager::Release() {
    for(map<string,Image*>::iterator it = image_memory_.begin();
            it != image_memory_.end(); ++it) {
        Image* img = it->second;
        img->Destroy();
        delete img;
    }
    image_memory_.clear();

    for(map<string,Image*>::iterator it = image_memory_.begin();
            it != image_memory_.end(); ++it) {
        Image* img = it->second;
        img->Destroy();
        delete img;
    }
    image_memory_.clear();
    return true;
}

// Desenha backbuffer na tela
void VideoManager::Render() {
    //Update screen
    SDL_GL_SwapBuffers();
    glClear( GL_COLOR_BUFFER_BIT );
}

Texture* VideoManager::LoadTextureFromFile(const string& filepath) {
    if(memory_.count(filepath) == 0) {
        Texture* tex = new Texture;
        if(tex == NULL)
            return NULL;
        if(!tex->LoadFromFile(filepath)) {
            delete tex;
            return NULL;
        }
        memory_[filepath] = tex;
    }
    return memory_[filepath];
}

// Carrega imagem de um arquivo, fazendo o
// gerenciamento de memoria. Retorna NULL
// em caso de falha.
Image* VideoManager::LoadImageFile(const string& filepath) {
    if(image_memory_.count(filepath) == 0) {
        Texture* tex = LoadTextureFromFile(filepath);
        if(tex == NULL)
            return NULL;
        image_memory_[filepath] = new Image(tex, false);
    }
    return image_memory_[filepath];
}

}  // namespace framework
