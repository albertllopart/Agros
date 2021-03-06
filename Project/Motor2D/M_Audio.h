#ifndef __M_AUDIO_H__
#define __M_AUDIO_H__

#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include "p2List.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f
#define MAX_CHANNELS 32

struct _Mix_Music;
struct Mix_Chunk;

class Audio : public Module
{
public:

	Audio();

	// Destructor
	virtual ~Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	bool Save(pugi::xml_node& data) const;
	bool Load(pugi::xml_node& data);

	void MusicUp();
	void MusicDown();
	void FxUp();
	void FxDown();

private:

	uint				music_volume;
	uint				fx_volume;

	bool				volume_changed = false;

	_Mix_Music*			music = nullptr;
	p2List<Mix_Chunk*>	fx;
};

#endif // __M_AUDIO_H__