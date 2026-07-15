#pragma once
#include "Actor.h"

class Player : public nu::Actor {
	public:
		Player() = default;
		Player(float speed, const nu::Transform& transform) : Actor{ transform }, a_speed{ speed } {}
		Player(float speed, const nu::Transform& transform, const nu::Model& model) : Actor{ transform, model }, a_speed{ speed } {}

		void Update(float dt) override;
		void Draw(const class nu::Renderer& renderer) const override;


	private:
		int a_amo = 0;
		float a_speed = 0.0f;
};