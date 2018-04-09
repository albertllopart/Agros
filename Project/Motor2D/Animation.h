#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "SDL/include/SDL_rect.h"
#include "Application.h"
#define FRAMES 50

class Animation
{
public:

	float				speed = 1.0f;
	SDL_Rect			frames[FRAMES];
	int					last_frame = 0;
	
private:

	float				current_frame = 1;

public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[last_frame++] = rect;
	}

	SDL_Rect& GetCurrentFrame()
	{
		current_frame = current_frame + speed; //App->GetDT();
		if (current_frame >= last_frame)
		{
			current_frame = 0;
		}
		return frames[(int)current_frame];
	}

};

#endif