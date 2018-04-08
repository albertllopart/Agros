#include "p2Defs.h"
#include "p2Log.h"
#include "Application.h"
#include "M_Input.h"
#include "M_Textures.h"
#include "M_Audio.h"
#include "M_Render.h"
#include "M_Window.h"
#include "M_Scene.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

Audio::Audio() : Module()
{
	music = NULL;
	name.create("audio");
}

// Destructor
Audio::~Audio()
{}

// Called before render is available
bool Audio::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	//XML config
	music_volume = config.child("volume").attribute("music").as_uint();
	fx_volume = config.child("volume").attribute("fx").as_uint();

	Mix_AllocateChannels(MAX_CHANNELS);
	Mix_VolumeMusic(music_volume);

	for (int i = 0; i <= MAX_CHANNELS; i++)
	{
		Mix_Volume(i, fx_volume);
	}

	return ret;
}

bool Audio::Update(float dt)
{
	//Volume control
	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN)
	{
		App->audio->MusicUp();
		App->audio->FxUp();
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN)
	{
		App->audio->MusicDown();
		App->audio->FxDown();
	}

	if (volume_changed == true)
	{
		Mix_VolumeMusic(music_volume);
		volume_changed = false;
	}

	return true;
}

// Called before quitting
bool Audio::CleanUp()
{
	if (!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if (music != NULL)
	{
		Mix_FreeMusic(music);
	}

	p2List_item<Mix_Chunk*>* item;
	for (item = fx.start; item != NULL; item = item->next)
		Mix_FreeChunk(item->data);

	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool Audio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if (!active)
		return false;

	if (music != NULL)
	{
		if (fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if (music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if (fade_time > 0.0f)
		{
			if (Mix_FadeInMusic(music, -1, (int)(fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if (Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

// Load WAV
unsigned int Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if (!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if (chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.add(chunk);
		ret = fx.count();
	}

	return ret;
}

// Play WAV
bool Audio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	if (!active)
		return false;

	if (id > 0 && id <= fx.count())
	{
		Mix_PlayChannel(-1, fx[id - 1], repeat);
	}

	return ret;
}

void Audio::MusicUp()
{
	if (music_volume < 128)
	{
		music_volume += 16;
		volume_changed = true;
	}
}

void Audio::MusicDown()
{
	if (music_volume > 0)
	{
		music_volume -= 16;
		volume_changed = true;
	}
}

void Audio::FxUp()
{
	if (fx_volume < 128)
	{
		fx_volume += 32;
	}
}

void Audio::FxDown()
{
	if (fx_volume > 0)
	{
		fx_volume -= 32;
	}
}

bool Audio::Save(pugi::xml_node& data) const
{
	pugi::xml_node volume = data.append_child("volume");

	volume.append_attribute("music") = music_volume;
	volume.append_attribute("fx") = fx_volume;

	return true;
}

bool Audio::Load(pugi::xml_node& data)
{
	music_volume = data.child("volume").attribute("music").as_uint();
	fx_volume = data.child("volume").attribute("fx").as_uint();

	volume_changed = true;

	return true;
}