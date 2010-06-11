
#include "../../framework/engine.h"
#include "../../framework/scene.h"
#include "../../framework/image.h"
#include "../../framework/sprite.h"
#include "../../framework/videomanager.h"
#include "../../framework/inputmanager.h"
#include "../../framework/timehandler.h"
#include "../../framework/vector2D.h"
#include "hero.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace framework;


namespace sprite {

Hero::Hero() {
    video_ = Engine::reference()->video_manager();
	set_image(Engine::reference()->video_manager()->LoadImage("mage.png"));
	set_position(Vector2D(280, 280));	

	directions_[Direction_::RIGHT] = Vector2D(1, 0);
	directions_[Direction_::LEFT] = Vector2D(-1, 0);
	directions_[Direction_::DOWN] = Vector2D(0, 1);
	directions_[Direction_::UP] = Vector2D(0, -1);

	last_standing_animation_ = new Animation(0, -1);
	for (int i = 0; i < 16; i++) {
		standing_animations_[i] = (Animation **) malloc (sizeof (Animation *));
		walking_animations_[i] = (Animation **) malloc (sizeof (Animation *));
		*standing_animations_[i] = NULL;
		*walking_animations_[i] = NULL;
	}
	*standing_animations_[Animation_::DOWN] = new Animation(0, 0, -1);
	*standing_animations_[Animation_::LEFT] = new Animation(0, 5, -1);
	*standing_animations_[Animation_::RIGHT] = new Animation(0, 10, -1);
	*standing_animations_[Animation_::UP] = new Animation(0, 15, -1);
	/**
	 * Por hora, sao as mesmas animacoes. Quando colocarmos as outras posicoes
	 * no sprite sheet, atualizaremos as animacoes.
	 */
	*standing_animations_[Animation_::DOWN | Animation_::RIGHT] = new Animation(0, 0, -1);
	*standing_animations_[Animation_::DOWN | Animation_::LEFT] = new Animation(0, 5, -1);
	*standing_animations_[Animation_::UP | Animation_::RIGHT] = new Animation(0, 10, -1);
	*standing_animations_[Animation_::UP | Animation_::LEFT] = new Animation(0, 15, -1);

	*walking_animations_[Animation_::DOWN] = new Animation(10, 0, 1, 2, 3, 4, -1);
	*walking_animations_[Animation_::LEFT] = new Animation(10, 5, 6, 7, 8, 9, -1);
	*walking_animations_[Animation_::RIGHT] = new Animation(10, 10, 11, 12, 13, 14, -1);
	*walking_animations_[Animation_::UP] = new Animation(10, 15, 16, 17, 18, 19, -1);
	/**
	 * Por hora, sao as mesmas animacoes. Quando colocarmos as outras posicoes
	 * no sprite sheet, atualizaremos as animacoes.
	 */
	*walking_animations_[Animation_::DOWN | Animation_::RIGHT] = new Animation(0, 0, -1);
	*walking_animations_[Animation_::DOWN | Animation_::LEFT] = new Animation(0, 5, -1);
	*walking_animations_[Animation_::UP | Animation_::RIGHT] = new Animation(0, 10, -1);
	*walking_animations_[Animation_::UP | Animation_::LEFT] = new Animation(0, 15, -1);

	animation_direction_ = 0;
	last_standing_animation_ = *standing_animations_[Animation_::DOWN];
	for (int i = 0; i < 16; i++) {
		if (*standing_animations_[i] == NULL) {
	//		free(standing_animations_[i]);
			standing_animations_[i] = &last_standing_animation_;
		}
		if (*walking_animations_[i] == NULL) {
	//		free(walking_animations_[i]);
			walking_animations_[i] = &last_standing_animation_;
		}
	}

	for (int i = 0; i < 4; i++) {
		pressed_key_[i] = false;
	}
    SelectSpriteAnimation();
}

void Hero::SelectSpriteAnimation() {
	Vector2D frame_size(74, 60);
	this->image()->set_frame_size(frame_size);
	this->SelectAnimation(*walking_animations_[animation_direction_]);
}

void Hero::Move() {
	float speed = 10;

	Vector2D position(this->position().x, this->position().y);
	Vector2D dir (0, 0);
	for (int i = 0; i < 4; i++) {
		if (pressed_key_[i]) {
			dir = dir + directions_[i];
		}
	}
	dir = Vector2D::Normalized(dir);
	dir = dir * speed;
	position = position + dir;

	set_position(position);
}

void Hero::Get_keys() {
	InputManager *input_ = Engine::reference()->input_manager();
	// -----------------------------
	// Nao copiar esta parte
	if(input_->KeyDown(K_ESCAPE))
		Engine::reference()->quit();
	//        if(input_->KeyDown(SDLK_SPACE))
	//            mage_->Spell();
	// -----------------------------

	for (int i = 0; i < 4; i++) {
		pressed_key_[i] = false;
	}

	animation_direction_ = 0;
	if(input_->KeyDown(K_w)) {
		pressed_key_[Direction_::UP] = true;
		animation_direction_ += Animation_::UP;
	}
	if(input_->KeyDown(K_a)) {
		pressed_key_[Direction_::LEFT] = true;
		animation_direction_ += Animation_::LEFT;
	}
	if(input_->KeyDown(K_s)) {
		pressed_key_[Direction_::DOWN] = true;
		animation_direction_ += Animation_::DOWN;
	}
	if(input_->KeyDown(K_d)) {
		pressed_key_[Direction_::RIGHT] = true;
		animation_direction_ += Animation_::RIGHT;
	}

	last_standing_animation_ = *(standing_animations_[animation_direction_]);
}

void Hero::Update(float delta_t) {
	Creature::Update(delta_t);
	video_->backbuffer()->Clear(0);
	this->Get_keys();
	this->Move();
	this->SelectSpriteAnimation();
}
}
