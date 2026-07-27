#pragma once

#include <fmod.hpp>

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace nu {
	class AudioSystem {
	public:
		AudioSystem() = default;

		bool Initialize();
		void Shutdown();
		void Update();

		// Original index-based sound functions
		bool LoadSound(const char* filename);
		void PlaySound(std::size_t index);

		// New named sound functions
		bool LoadSound(
			const std::string& name,
			const std::string& filename
		);

		void PlaySound(const std::string& name);

		bool HasSound(const std::string& name) const;

		std::size_t GetSoundCount() const {
			return a_sounds.size();
		}

	private:
		bool CreateSound(
			const char* filename,
			FMOD::Sound*& sound
		);

	private:
		FMOD::System* a_audio = nullptr;

		std::vector<FMOD::Sound*> a_sounds;

		std::unordered_map<
			std::string,
			std::size_t
		> a_soundIndexes;
	};
}