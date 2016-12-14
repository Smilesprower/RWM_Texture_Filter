#pragma once
#define _USE_MATH_DEFINES
#include <map>
#include <SDL.h>
#include <SDL_image.h>
#include "JTexture.h"
#include <cmath>
#include <vector>
class TextureFilter
{
public:
	TextureFilter();
	~TextureFilter();

	// Creates and loads a JTexture which allows pixel manipulation
	bool TextureFilter::createJtexture(std::string fileName, int id, SDL_Renderer* renderer);
	// Returns the SDL_Texture from JTexture
	SDL_Texture* getTexture(int id);
	// Returns the the bounds of the texture from JTexture
	SDL_Rect getRectBounds(int id);
	// Grayscale filter 
	void grayscaleFilter(int id);
	void edgeDectection(int id);
	void resetPixels(int id);
	void calcGaussianKernel(int length, float weight);

private:
	const Uint8 MAX_KERNAL_SIZE = 9;

	std::map<int, JTexture*> m_textureMap;
	std::vector<std::vector<float>> m_gaussianKernel;	
};

