#include "stdafx.h"
#include "Application.h"

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;


Application::Application()
	: m_quit(false)
	, m_lastTime(LTimer::gameTime())
{
}


Application::~Application()
{
}

bool Application::Initialize(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "SDL init success" << std::endl;
		m_window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		if (m_window != 0)
		{
			std::cout << "Window creation success" << std::endl;
			m_renderer = SDL_CreateRenderer(m_window, -1, 0);
			if (m_renderer != 0)
			{
				std::cout << "Renderer creation success" << std::endl;
				SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
			}
			else
			{
				std::cout << "Renderer init fail" << std::endl;
				return false;
			}
		}
		else
		{
			std::cout << "Window init fail" << std::endl;
			return false;
		}
	}
	else
	{
		std::cout << "SDL init fail" << std::endl;
		return false;
	}

	LoadContent();
	return true;
}

void Application::CleanUp()
{
	std::cout << "Cleaning Up" << std::endl;
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_Quit();
}

void Application::Loop()
{
	LTimer capTimer;
	int frameNum = 0;

	while (!m_quit)
	{
		capTimer.start();

		HandleEvents();
		Update();
		Render();

		int frameTicks = capTimer.getTicks();
		if (frameTicks < SCREEN_TICKS_PER_FRAME)
		{
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
	}
}

void Application::LoadContent()
{
	if (m_texFilter.createJtexture("assets/tiles.png", 0, m_renderer))
	{
		std::cout << "JTexture created and image loaded" << std::endl;
	}
	else
	{
		std::cout << "Jtexture was not created and image was not loaded" << std::endl;
	}
}

void Application::UnloadContent()
{
}

void Application::Render()
{
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_renderer);


	SDL_RenderCopy(m_renderer, m_texFilter.getTexture(0), NULL, &m_texFilter.getRectBounds(0));

	SDL_RenderPresent(m_renderer);
}

void Application::Update()
{
	unsigned int currentTime = LTimer::gameTime();
	float deltaTime = (currentTime - m_lastTime) / 1000.f;
	// Update in here
	m_lastTime = currentTime;
}

void Application::HandleEvents()
{
	while (SDL_PollEvent(&m_event) != 0)
	{
		if (m_event.type == SDL_QUIT)
		{
			m_quit = true;
		}
		else
		{
			switch (m_event.type)
			{
				case SDL_KEYDOWN:
				switch (m_event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						m_quit = true;
						break;
					case SDLK_1:
						m_texFilter.grayscaleFilter(0);
						break;
					case SDLK_2:
						m_texFilter.resetPixels(0);
						break;
					case SDLK_3:
						m_texFilter.pixelateFilter(0);
						break;
					case SDLK_4:
						m_texFilter.gaussianBlur1D(0, 11, 2.0);
						break;
				}
			}
		}
	}
}
