#include <stdlib.h>
#include <crtdbg.h>
#ifdef _MSC_VER
#else
#define _ASSERT(expr) ((void)0)

#define _ASSERTE(expr) ((void)0)
#endif

#include "p2Defs.h"
#include "p2Log.h"
#include "Application.h"

// This is needed here because SDL redefines main function
// do not add any other libraries here, instead put them in their modules
#include "SDL/include/SDL.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

//brofiler
#include "Brofiler\Brofiler.h"
#pragma comment(lib, "Brofiler/ProfilerCore32.lib")

//pugixml
#include "PugiXml\src\pugixml.hpp"

enum MainState
{
	CREATE = 1,
	AWAKE,
	START,
	UPDATE,
	CLEAN,
	FAIL,
	EXIT
};

Application* App = NULL;

int main(int argc, char* args[])
{
	LOG("Engine starting...");

	MainState state = MainState::CREATE;
	int result = EXIT_FAILURE;

	while (state != EXIT)
	{
		switch (state)
		{

			//Allocate the engine
			case CREATE:
			{
				LOG("Creation phase");

				App = new Application(argc, args);

				if (App != NULL)
				{
					state = AWAKE;
				}
				else
				{
					state = FAIL;
				}
				break;
			}

			//Awake all modules
			case AWAKE:
			{
				LOG("Awake phase");

				if (App->Awake() == true)
				{
					state = START;
				}
				else
				{
					LOG("Error: Awake failed");
					state = FAIL;
				}
				break;
			}

			//Call all modules before first frame
			case START:
			{
				LOG("Start phase");

				if (App->Start() == true)
				{
					state = UPDATE;
					LOG("Update phase");
				}
				else
				{
					state = FAIL;
					LOG("Error: Start failed");
				}
				break;
			}

			//Loop all modules until we are asked to leave
			case UPDATE:
			{
				if (App->Update() == false)
				{
					state = CLEAN;
				}
				break;
			}

			//Cleanup allocated memory
			case CLEAN:
			{
				LOG("Cleanup Phase");

				if (App->CleanUp() == true)
				{
					RELEASE(App);
					result = EXIT_SUCCESS;
					state = EXIT;
				}
				else
				{
					state = FAIL;
				}
				break;
			}

			//Exit with errors
			case FAIL:
			{
				LOG("Exiting with errors");
				result = EXIT_FAILURE;
				state = EXIT;
				break;
			}
		}
	}

	LOG("Exiting");

	return result;
}