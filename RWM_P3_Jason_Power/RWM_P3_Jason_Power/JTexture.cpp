#include "stdafx.h"
#include "JTexture.h"


JTexture::JTexture()
	: m_texture(NULL)
	, m_pixels(NULL)
	, m_width(0)
	, m_height(0)
	, m_pitch(0)
	, m_dest(SDL_Rect{0,0,0,0})
{
}


JTexture::~JTexture()
{
}

SDL_Texture * JTexture::getTexture()
{
	return m_texture;
}

SDL_Rect JTexture::getDestRect()
{
	return m_dest;
}

void * JTexture::getPixels()
{
	return m_pixels;
}

SDL_Surface * JTexture::getSurface()
{
	return m_surface;
}
bool JTexture::loadFromFile(std::string fileName, int id, SDL_Renderer* renderer)
{
	SDL_Texture* newTexture = NULL;
	m_surface = IMG_Load(fileName.c_str());

	newTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, m_surface->w, m_surface->h);
	//Lock texture for manipulation
	SDL_LockTexture(newTexture, NULL, &m_pixels, &m_pitch);
	//Copy loaded/formatted surface pixels
	memcpy(m_pixels, m_surface->pixels, m_surface->pitch * m_surface->h);
	//Unlock texture to update
	SDL_UnlockTexture(newTexture);

	// Enables Transparency 
	SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);

	//Return success
	m_texture = newTexture;
	return true;
}

void JTexture::copyPixelData()
{
	memcpy(m_pixels, m_surface->pixels, m_surface->pitch * m_surface->h);
}

