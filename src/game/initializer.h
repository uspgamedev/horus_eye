#ifndef HORUSEYE_INITIALIZER_H_
#define HORUSEYE_INITIALIZER_H_

#include <string>
#include <ugdk/action.h>
#include <ugdk/graphic.h>

void AddHorusShader();
bool VerifyFolderExists(const std::string& path);
ugdk::action::Scene* CreateHorusLightrenderingScene();

ugdk::graphic::opengl::ShaderProgram* get_horus_light_shader();

void ToggleShadowcasting();
void ToggleLightsystem();

void DrawTexture(ugdk::graphic::Texture* texture, const ugdk::graphic::Geometry& geometry, const ugdk::graphic::VisualEffect& effect);

#endif