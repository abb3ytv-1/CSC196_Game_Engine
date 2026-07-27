#include "Bullet.h"
#include "MathUtils.h"

/*
* Bullet description:
* - Moves forward in the direction it is facing
* - Uses a lifespan so it does not exist forever
* - Has no damping, so it keeps a constant speed
* - Can collide with other actors
*/

Bullet::Bullet(
	const nu::Transform& transform,
	const nu::Model& model,
	float speed,
	float lifespan
) :
	Actor{ transform, model }
{
	// The models face right, so positive x is forward.
	nu::Vector2 forward{ 1.0f, 0.0f };

	// Convert the bullet's rotation from degrees to radians.
	forward = forward.Rotate(
		transform.rotation * nu::DegToRad
	);

	SetVelocity(forward * speed);

	// Bullets should not slow down.
	SetDamping(0.0f);

	// Automatically destroy the bullet after this time.
	SetLifespan(lifespan);

	// This radius is multiplied by the bullet's scale.
	SetCollisionRadius(1.5f);
}

void Bullet::Update(float dt) {
	Actor::Update(dt);
}