#ifndef HORUSEYE_INITIALIZER_H_
#define HORUSEYE_INITIALIZER_H_

#include <string>
#include <ugdk/action.h>
#include <ugdk/graphic.h>
    
void QueueRestartGame();
bool RestartGameQueued();

void AddHorusShader();
bool VerifyFolderExists(const std::string& path);

ugdk::graphic::ShaderProgram* get_horus_light_shader();

#endif