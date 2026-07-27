#include "pch.h"
#include "AudioSystem.h"

#include <fmod_errors.h>

#include <iostream>

namespace nu {
	bool AudioSystem::Initialize() {
		FMOD_RESULT result =
			FMOD::System_Create(&a_audio);

		if (result != FMOD_OK) {
			std::cerr
				<< "FMOD System_Create failed: "
				<< FMOD_ErrorString(result)
				<< '\n';

			return false;
		}

		void* extraDriverData = nullptr;

		result = a_audio->init(
			32,
			FMOD_INIT_NORMAL,
			extraDriverData
		);

		if (result != FMOD_OK) {
			std::cerr
				<< "FMOD initialization failed: "
				<< FMOD_ErrorString(result)
				<< '\n';

			a_audio->release();
			a_audio = nullptr;

			return false;
		}

		return true;
	}

	void AudioSystem::Shutdown() {
		for (FMOD::Sound* sound : a_sounds) {
			if (sound != nullptr) {
				sound->release();
			}
		}

		a_sounds.clear();
		a_soundIndexes.clear();

		if (a_audio != nullptr) {
			a_audio->close();
			a_audio->release();
			a_audio = nullptr;
		}
	}

	void AudioSystem::Update() {
		if (a_audio != nullptr) {
			a_audio->update();
		}
	}

	bool AudioSystem::CreateSound(
		const char* filename,
		FMOD::Sound*& sound
	) {
		if (
			a_audio == nullptr ||
			filename == nullptr
			) {
			return false;
		}

		sound = nullptr;

		FMOD_RESULT result = a_audio->createSound(
			filename,
			FMOD_DEFAULT,
			nullptr,
			&sound
		);

		if (result != FMOD_OK) {
			std::cerr
				<< "Could not load "
				<< filename
				<< ": "
				<< FMOD_ErrorString(result)
				<< '\n';

			sound = nullptr;
			return false;
		}

		return true;
	}

	bool AudioSystem::LoadSound(const char* filename) {
		FMOD::Sound* sound = nullptr;

		if (!CreateSound(filename, sound)) {
			return false;
		}

		a_sounds.push_back(sound);

		return true;
	}

	bool AudioSystem::LoadSound(
		const std::string& name,
		const std::string& filename
	) {
		if (name.empty() || filename.empty()) {
			return false;
		}

		if (HasSound(name)) {
			std::cerr
				<< "A sound named '"
				<< name
				<< "' is already loaded.\n";

			return false;
		}

		FMOD::Sound* sound = nullptr;

		if (!CreateSound(filename.c_str(), sound)) {
			return false;
		}

		std::size_t index = a_sounds.size();

		a_sounds.push_back(sound);
		a_soundIndexes[name] = index;

		return true;
	}

	void AudioSystem::PlaySound(std::size_t index) {
		if (
			a_audio == nullptr ||
			index >= a_sounds.size() ||
			a_sounds[index] == nullptr
			) {
			return;
		}

		FMOD_RESULT result = a_audio->playSound(
			a_sounds[index],
			nullptr,
			false,
			nullptr
		);

		if (result != FMOD_OK) {
			std::cerr
				<< "FMOD playback failed: "
				<< FMOD_ErrorString(result)
				<< '\n';
		}
	}

	void AudioSystem::PlaySound(
		const std::string& name
	) {
		auto iterator = a_soundIndexes.find(name);

		if (iterator == a_soundIndexes.end()) {
			std::cerr
				<< "Could not find sound named '"
				<< name
				<< "'.\n";

			return;
		}

		PlaySound(iterator->second);
	}

	bool AudioSystem::HasSound(
		const std::string& name
	) const {
		return (
			a_soundIndexes.find(name) !=
			a_soundIndexes.end()
			);
	}
}