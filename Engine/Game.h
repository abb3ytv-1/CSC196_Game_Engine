#pragma once

class Scene;

namespace nu {
	class Game {
	public:
		Game() = default;
		Game(Scene* scene) : a_scene{scene} {}

		virtual bool Initialize() { return true; }
		virtual void Shutdown() {}

		virtual void Update(float dt);
		virtual void Draw(const class Renderer& renderer);

		void SetScene(Scene* scene) { a_scene = scene; }

	protected:
		Scene* a_scene;
	};
}