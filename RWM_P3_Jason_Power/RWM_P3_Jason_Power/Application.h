#pragma once
#include <iostream>
#include "SDL.h"
#include "LTimer.h"
#include "TextureFilter.h"

class Application
{
public:
	Application();
	~Application();
	bool Initialize(const char*, int, int, int, int, int);
	void CleanUp();
	void Loop();

private:
	bool m_quit;
	unsigned int m_lastTime;

	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	SDL_Event m_event;
	TextureFilter m_texFilter;

	void LoadContent();
	void UnloadContent();
	void Render();
	void Update();
	void HandleEvents();
};
