#include "stdafx.h"
#pragma once
#define SDL_MAIN_HANDLED
#ifdef __APPLE__
#include "SDL2/SDL.h"
#elif defined(_WIN64) || defined(_WIN32)
#include "SDL.h"
#endif

#pragma comment(lib,"SDL2.lib") 
#pragma comment(lib, "SDL2_image.lib")

#include <SDL_image.h>
#include "Application.h"
int main()
{
	Application* app = new Application();

	app->Initialize("RWM - Texture Filters", 500, 200, 800, 600, SDL_WINDOW_INPUT_FOCUS);
	app->Loop();
	app->CleanUp();
	system("Pause");
	return 0;
}

