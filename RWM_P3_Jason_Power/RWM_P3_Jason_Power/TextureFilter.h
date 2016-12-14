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
	void pixelateFilter(int id);
	void gaussianBlur1D(int id, int kernelSize, float sigma);
	void resetPixels(int id);
	void calc1DGaussianKernel(const int kernelSize, float sigma);

private:
	const Uint8 MAX_KERNAL_SIZE = 15;

	std::map<int, JTexture*> m_textureMap;
	std::vector<std::vector<float>> m_gaussianKernel;	
	std::vector<float> m_gaussianKernel1D;
	int m_currentKernelSize;
	double * m_kernelData;
};

