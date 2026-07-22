#include "pch.h"
#include "AudioSystem.h"

#include <fmod_errors.h>
#include <iostream>

namespace nu {
	bool AudioSystem::Initialize() {
		FMOD_RESULT result = FMOD::Sysyem_Create(&a_audio);

		if (result != FMOD_OK) {
			std::cerr
				<< "FMOD System_Create failed: "
				<< FMOD_ErrorString(result)
				<< "\n";

			return false;
		}

		void* extradriverdata = nullptr;

		result a_audio->init(
			32,
			FMOD_INIT_NORMAL,
			extradriverdata
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

	bool AudioSystem::LoadSound(const char* filename) {
		FMOD::Sound* sound = nullptr;

		FMOD_RESULT result = a_audio->createSound(
			filename,
			FMOD_DEFAULT,
			nullptr,
			&sound
		);

		a_sounds.push_back(sound);

		if (result != FMOD_OK) {
			std::cerr
				<< "Could not load " << filename << ": "
				<< FMOD_ErrorString(result)
				<< '\n';

			return false;
		}

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
}