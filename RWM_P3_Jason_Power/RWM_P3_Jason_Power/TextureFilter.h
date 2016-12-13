#pragma once
#include <map>
#include <SDL.h>
#include <SDL_image.h>
#include "JTexture.h"
class TextureFilter
{
public:
	TextureFilter();
	~TextureFilter();

	// Creates and loads a JTexture which allows pixel manipulation
	bool TextureFilter::createJtexture(std::string fileName, int id, SDL_Renderer* renderer);
	// Returns the SDL_Texture from m_textureMap
	SDL_Texture* getTexture(int id);
	// Returns the SDL_Rect "Texture Bounds Rect" from m_textureMap
	SDL_Rect getDestRect(int id);
	void greyEverything(int id);
	void resetPixels(int id);

private:
	std::map<int, JTexture*> m_textureMap;
};

