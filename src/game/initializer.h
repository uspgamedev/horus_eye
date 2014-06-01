#ifndef HORUSEYE_INITIALIZER_H_
#define HORUSEYE_INITIALIZER_H_

#include <string>
#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/internal.h>
    
void QueueRestartGame();
bool RestartGameQueued();

void AddHorusShader();
bool VerifyFolderExists(const std::string& path);
ugdk::action::Scene* CreateShadowCastingScene();
ugdk::action::Scene* CreateHorusLightrenderingScene();

ugdk::graphic::opengl::ShaderProgram* get_horus_light_shader();

void ToggleShadowcasting();
void ToggleLightsystem();

void DrawTexture(ugdk::internal::GLTexture* texture, ugdk::graphic::Canvas&);

#endif