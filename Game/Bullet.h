#pragma once

#include "Actor.h"

class Bullet : public nu::Actor {
public:
	Bullet() = default;

	Bullet(
		const nu::Transform& transform,
		const nu::Model& model,
		float speed = 700.0f,
		float lifespan = 2.0f
	);

	void Update(float dt) override;
};