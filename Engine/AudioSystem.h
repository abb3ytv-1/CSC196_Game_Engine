#pragma once
#include <fmod.hpp>

#include <cstddef>
#include <vector>

namespace nu {
	class AudioSystem {
	public:
		bool Initialize();
		void Shutdown();
		void Update();

		bool LoadSound(const char* filename);
		void PlaySound(std::size_t index);

	private:
		FMOD::System* a_audio = nullptr;
		std::vector<FMOD::Sound*> a_sounds;
	};
}