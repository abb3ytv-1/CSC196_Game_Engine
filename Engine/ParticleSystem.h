#pragma once

namespace nu
{
	struct Particle
	{
		bool active{ false };
		float lifespan{ 1 };

		Vector2 position{ 0, 0 };
		Vector2 velocity{ 0, 0 };
		Color color{ 0, 0, 0 };
	};
}