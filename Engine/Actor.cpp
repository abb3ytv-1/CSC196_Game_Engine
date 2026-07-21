#include "pch.h"
#include "Actor.h"
#include "Renderer.h"
#include "MathUtils.h"

namespace nu {
	void Actor::Update(float dt) {
		a_transform.position += (a_velocity * dt);
		a_velocity *= 0.997f;

		a_transform.position.x = Wrap(
			0.0f,
			1920.0f,
			a_transform.position.x
		);

		a_transform.position.y = Wrap(
			0.0f,
			1080.0f,
			a_transform.position.y
		);
	}

	void Actor::Draw(const Renderer& renderer) const {
		renderer.DrawModel(a_model, a_transform);
	}
}