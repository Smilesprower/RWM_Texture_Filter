#include "stdafx.h"
#include "TextureFilter.h"


TextureFilter::TextureFilter()
{
}


TextureFilter::~TextureFilter()
{
}

bool TextureFilter::createJtexture(std::string fileName, int id, SDL_Renderer * renderer)
{
	JTexture* temp = new JTexture();
	if (temp->loadFromFile(fileName, id, renderer))
	{
		m_textureMap[id] = temp;
		return true;
	}
	return false;
}

SDL_Texture * TextureFilter::getTexture(int id)
{
	return m_textureMap[id]->getTexture();
}

SDL_Rect TextureFilter::getDestRect(int id)
{
	return m_textureMap[id]->getDestRect();
}

void TextureFilter::greyEverything(int id)
{
	SDL_Texture* tempTex = m_textureMap[id]->getTexture();
	int tempPitch = 0;
	void* tempPixels = NULL;

	SDL_LockTexture(tempTex, NULL, &tempPixels, &tempPitch);

	Uint32* pixels = (Uint32*)tempPixels;

	int pixelCount = (m_textureMap[id]->getSurface()->pitch * 0.25f) *  m_textureMap[id]->getSurface()->h;
	for (int i = 0; i < pixelCount; ++i)
	{
		Uint8 a = pixels[i] >> 24 & 0xFF;
		if (a != 0)
		{
			Uint8 r = pixels[i] >> 16 & 0xFF;
			Uint8 g = pixels[i] >> 8 & 0xFF;
			Uint8 b = pixels[i] & 0xFF;
			Uint8 v = 0.212671f * r + 0.715160f * g + 0.072169f * b;
			pixels[i] = (0xFF << 24) | (v << 16) | (v << 8) | v;
		}
	}
	SDL_UnlockTexture(tempTex);
	// Add a flag for transparency for the user
	SDL_SetTextureBlendMode(tempTex, SDL_BLENDMODE_BLEND);
}

void TextureFilter::resetPixels(int id)
{
	SDL_Texture* tempTex = m_textureMap[id]->getTexture();
	SDL_Surface * tempSurf = m_textureMap[id]->getSurface();
	int pitch = 0;
	void* tempPixels = NULL;
	SDL_LockTexture(tempTex, NULL, &tempPixels, &pitch);
	memcpy(tempPixels, tempSurf->pixels, tempSurf->pitch * tempSurf->h);
	SDL_UnlockTexture(tempTex);
}
