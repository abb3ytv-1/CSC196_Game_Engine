#include "Player.h"
#include "Renderer.h"
#include "engine.h"
#include "Assets.h"
#include "Bullet.h"

/*
* Things to add:
* - Movement
* - Rotate
* - Bullet
* - bullet descripton
*/

void Player::Update(float dt) {

	nu::Vector2 force{ 0.0f, 0.0f };
	if (nu::engine.GetInput().GetKeyDown(SDL_SCANCODE_W)) force.x = -a_speed;
	if (nu::engine.GetInput().GetKeyDown(SDL_SCANCODE_S)) force.x = +a_speed;
	if (nu::engine.GetInput().GetKeyDown(SDL_SCANCODE_A)) force.y = -a_speed;
	if (nu::engine.GetInput().GetKeyDown(SDL_SCANCODE_D)) force.y = -a_speed;

	SetVelocity(GetVelocity() + (force * dt));

	Actor::Update(dt);
}

void Player::Draw(const nu::Renderer& renderer) const {
	Actor::Draw(renderer);
}