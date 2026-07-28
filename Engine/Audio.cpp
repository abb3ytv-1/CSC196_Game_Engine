#include "pch.h"
#include "Audio.h"

#include <fmod_errors.h>

#include <iostream>

namespace nu {
	bool Audio::Initialize() {
		FMOD_RESULT result =
			FMOD::System_Create(&m_fmodSystem);

		if (!CheckFMODResult(result)) {
			return false;
		}

		void* extraDriverData = nullptr;

		result = m_fmodSystem->init(
			32,
			FMOD_INIT_NORMAL,
			extraDriverData
		);

		if (!CheckFMODResult(result)) {
			m_fmodSystem->release();
			m_fmodSystem = nullptr;

			return false;
		}

		return true;
	}

	void Audio::Shutdown() {
		// Release all loaded sounds.
		for (auto& soundPair : m_sounds) {
			FMOD::Sound* sound = soundPair.second;

			if (sound != nullptr) {
				CheckFMODResult(sound->release());
			}
		}

		m_sounds.clear();

		if (m_fmodSystem != nullptr) {
			CheckFMODResult(m_fmodSystem->close());
			CheckFMODResult(m_fmodSystem->release());

			m_fmodSystem = nullptr;
		}
	}

	void Audio::Update() {
		if (m_fmodSystem != nullptr) {
			CheckFMODResult(
				m_fmodSystem->update()
			);
		}
	}

	bool Audio::AddSound(
		const std::string& name,
		const std::string& filename
	) {
		// Check whether the sound name already exists.
		if (m_sounds.find(name) != m_sounds.end()) {
			std::cerr
				<< "Audio System: name already exists: "
				<< name
				<< '\n';

			return false;
		}

		if (m_fmodSystem == nullptr) {
			return false;
		}

		FMOD::Sound* sound = nullptr;

		FMOD_RESULT result =
			m_fmodSystem->createSound(
				filename.c_str(),
				FMOD_DEFAULT,
				nullptr,
				&sound
			);

		if (!CheckFMODResult(result)) {
			return false;
		}

		// Store the sound pointer using its descriptive name.
		m_sounds[name] = sound;

		return true;
	}

	bool Audio::PlaySound(const std::string& name) {
		auto soundIterator = m_sounds.find(name);

		if (soundIterator == m_sounds.end()) {
			std::cerr
				<< "Audio System: name does not exist: "
				<< name
				<< '\n';

			return false;
		}

		if (m_fmodSystem == nullptr) {
			return false;
		}

		FMOD_RESULT result =
			m_fmodSystem->playSound(
				soundIterator->second,
				nullptr,
				false,
				nullptr
			);

		if (!CheckFMODResult(result)) {
			return false;
		}

		return true;
	}

	bool Audio::CheckFMODResult(
		FMOD_RESULT result
	) {
		if (result != FMOD_OK) {
			std::cerr
				<< "FMOD Error: "
				<< FMOD_ErrorString(result)
				<< '\n';

			return false;
		}

		return true;
	}
}