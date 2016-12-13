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
	SDL_Surface * tempSurf = m_textureMap[id]->getSurface();
	SDL_Rect * tempRect = &m_textureMap[id]->getDestRect();
	Uint32* tempPixels = (Uint32*)m_textureMap[id]->getPixels();

	int pixelCount = (tempSurf->pitch * 0.25f) *  tempSurf->h;
	for (int i = 0; i < pixelCount; ++i)
	{
		Uint8 a = tempPixels[i] >> 24 & 0xFF;
		if (a != 0)
		{
			Uint8 r = tempPixels[i] >> 16 & 0xFF;
			Uint8 g = tempPixels[i] >> 8 & 0xFF;
			Uint8 b = tempPixels[i] & 0xFF;
			Uint8 v = 0.212671f * r + 0.715160f * g + 0.072169f * b;
			tempPixels[i] = (0xFF << 24) | (v << 16) | (v << 8) | v;
		}
	}
	SDL_UpdateTexture(tempTex, tempRect, tempPixels, tempSurf->pitch);

	// Add a flag for transparency for the user
	SDL_SetTextureBlendMode(tempTex, SDL_BLENDMODE_BLEND);
}

void TextureFilter::resetPixels(int id)
{
	SDL_Texture* tempTex = m_textureMap[id]->getTexture();
	SDL_Surface * tempSurf = m_textureMap[id]->getSurface();
	SDL_Rect * tempRect = &m_textureMap[id]->getDestRect();
	void* tempPixels = tempSurf->pixels;

	SDL_UpdateTexture(tempTex, tempRect, tempPixels, tempSurf->pitch);

}
