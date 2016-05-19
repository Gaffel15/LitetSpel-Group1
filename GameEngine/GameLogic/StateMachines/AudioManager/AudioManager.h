#pragma once
#include "../Audio/SoundCommon.h"
#include "../Audio/WAVFileReader.h"
#include "../../../Source/LibIncluder.h"
#include <Audio.h>
#include "../GameTimer/GameTimer.h"
using namespace DirectX;
class AudioManager
{
private:
	std::unique_ptr<AudioEngine>* s_audEngine = nullptr;

	std::unique_ptr<SoundEffect> s_shot;
	std::unique_ptr<SoundEffect> s_ambient;
	std::unique_ptr<SoundEffect> s_gameTheme;
	std::unique_ptr<SoundEffect> s_E_death;
	std::unique_ptr<SoundEffect> s_weaponPickup;
	std::unique_ptr<SoundEffect> s_healthPickup;
	std::unique_ptr<SoundEffect> s_imminentWave;
	std::unique_ptr<SoundEffect> s_loseAmient;
	std::unique_ptr<SoundEffectInstance> s_nightLoop;
	std::unique_ptr<SoundEffectInstance> s_musicLoop;
	std::unique_ptr<SoundEffectInstance> s_loseLoop;
	bool s_retryAudio = false;
public:
	AudioManager();
	~AudioManager();
	void Release();
	void Initialize();
	void Update(double deltaTime);

	//functions for playing the sounds
	void playShotSound();
	void playEDeathSound();
	void playHealthPickup();
	void playWeaponPickup();
	void playNewWave();
	void playInGameLoop();

	void stopAmbientGameStateSound();
};

