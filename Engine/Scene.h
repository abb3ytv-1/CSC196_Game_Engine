#pragma once

#include "Actor.h"

#include <cstddef>
#include <memory>
#include <vector>

namespace nu {
	class Renderer;

	class Scene {
	public:
		Scene() = default;

		void AddActor(std::unique_ptr<Actor> actor);

		void Update(float dt);
		void Draw(const Renderer& renderer) const;

		void RemoveAll();

		std::size_t GetActorCount() const {
			return a_actors.size();
		}

		std::size_t GetPendingActorCount() const {
			return a_pendingActors.size();
		}

	private:
		void AddPendingActors();
		void RemoveDestroyedActors();

	private:
		std::vector<std::unique_ptr<Actor>> a_actors;
		std::vector<std::unique_ptr<Actor>> a_pendingActors;
	};
}