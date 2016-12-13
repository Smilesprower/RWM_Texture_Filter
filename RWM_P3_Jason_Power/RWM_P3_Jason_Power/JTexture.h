#pragma once
#include "SDL.h"
#include <SDL_image.h>
#include <string>
#include <iostream>

class JTexture
{
public:
	JTexture();
	~JTexture();

	SDL_Surface* getSurface();
	SDL_Texture* getTexture();
	SDL_Rect getDestRect();
	void* getPixels();
	bool loadFromFile(std::string fileName, int id, SDL_Renderer* renderer);
	bool lockTexture();
	bool unlockTexture();

	int getPitch();
	int getHeight();
	int getWidth();


private:
	//The actual hardware texture
	SDL_Texture* m_texture;
	SDL_Surface* m_surface;
	SDL_Rect m_dest;
	Uint32* originalPixels;
	void* m_pixels;
	int m_pitch;

	//Image dimensions
	int m_width;
	int m_height;
};

