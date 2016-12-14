#include "stdafx.h"
#include "TextureFilter.h"


TextureFilter::TextureFilter()
	:m_gaussianKernel(MAX_KERNAL_SIZE, std::vector<float>(MAX_KERNAL_SIZE))
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

SDL_Rect TextureFilter::getRectBounds(int id)
{
	return m_textureMap[id]->getSurface()->clip_rect;
}

void TextureFilter::grayscaleFilter(int id)
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
	SDL_UpdateTexture(tempTex, &tempSurf->clip_rect, tempPixels, tempSurf->pitch);

	SDL_SetTextureBlendMode(tempTex, SDL_BLENDMODE_BLEND);
}
void TextureFilter::edgeDectection(int id)
{
	SDL_Texture* tempTex = m_textureMap[id]->getTexture();
	SDL_Surface * tempSurf = m_textureMap[id]->getSurface();
	Uint32* tempPixels = (Uint32*)m_textureMap[id]->getPixels();

	Uint8 r; 
	Uint8 g;
	Uint8 b;

	//float blur[3][3] = {		{ m_gaussianKernel[0][0], m_gaussianKernel[0][1], m_gaussianKernel[0][2] },
	//							{ m_gaussianKernel[1][0], m_gaussianKernel[1][1], m_gaussianKernel[1][2] },
	//							{ m_gaussianKernel[2][0], m_gaussianKernel[2][1], m_gaussianKernel[2][2] }
	//};

	//int emboss_filter[3][3] = { { 0,0,0 },{ 0,1,0 },{ 0,0,-1 } };
	//int edge_filter[3][3] = { { -1,-1,-1 },{ -1,8,-1 },{ -1,-1,-1 } };

	int imageWidth = tempSurf->pitch * 0.25f;
	int imageHeight = tempSurf->h;

	//for (int x = 1; x < imageWidth - 1; x++)
	//{
	//	for (int y = 1; y < imageHeight - 1; y++)
	//	{
	//		float sumr = 0;
	//		float sumb = 0;
	//		float sumg = 0;

	//		for (int k = -1; k < 2; k++)
	//		{
	//			for (int l = -1; l < 2; l++)
	//			{
	//				int pos = ((y + l) *imageWidth) + (x + k);
	//				r = tempPixels[pos] >> 16 & 0xFF;
	//				g = tempPixels[pos] >> 8 & 0xFF;
	//				b = tempPixels[pos] & 0xFF;

	//				sumr += r*blur[k + 1][l + 1];
	//				sumg += g*blur[k + 1][l + 1];
	//				sumb += b*blur[k + 1][l + 1];
	//			}
	//		}

	//		//if (sumr<0) sumr = 0;
	//		//if (sumb<0) sumb = 0;
	//		//if (sumg<0) sumg = 0;

	//		//if (sumr>255) sumr = 255;
	//		//if (sumb>255) sumb = 255;
	//		//if (sumg>255) sumg = 255;

	//		tempPixels[y * imageWidth + x] = (0xFF << 24) | (Uint8(sumr) << 16) | (Uint8(sumg) << 8) | Uint8(sumb);
	//	}
	//}

	double coeffs[] = { 0.018816,	0.034474,	0.056577,	0.083173,	0.109523,	0.129188,	0.136498,	0.129188,	0.109523,	0.083173,	0.056577,	0.034474,	0.018816 };
	for (int x = 6; x < imageWidth - 6; x++)
	{
		for (int y = 6; y < imageHeight - 6; y++)
		{
			float sumr = 0;
			float sumb = 0;
			float sumg = 0;

			for (int i = -6; i < 7; i++)
			{
				int pos = ((y + i) *imageWidth) + x;
				r = tempPixels[pos] >> 16 & 0xFF;
				g = tempPixels[pos] >> 8 & 0xFF;
				b = tempPixels[pos] & 0xFF;
				sumr += r*coeffs[i + 6];
				sumg += g*coeffs[i + 6];
				sumb += b*coeffs[i + 6];
			}
			tempPixels[y * imageWidth + x] = (0xFF << 24) | (Uint8(sumr) << 16) | (Uint8(sumg) << 8) | Uint8(sumb);
		}
	}
	for (int x = 6; x < imageWidth - 6; x++)
	{
		for (int y = 6; y < imageHeight - 6; y++)
		{
			float sumr = 0;
			float sumb = 0;
			float sumg = 0;

			for (int i = -6; i < 7; i++)
			{
				int pos = (y *imageWidth) + x + i;
				r = tempPixels[pos] >> 16 & 0xFF;
				g = tempPixels[pos] >> 8 & 0xFF;
				b = tempPixels[pos] & 0xFF;
				sumr += r*coeffs[i + 6];
				sumg += g*coeffs[i + 6];
				sumb += b*coeffs[i + 6];
			}
			tempPixels[y * imageWidth + x] = (0xFF << 24) | (Uint8(sumr) << 16) | (Uint8(sumg) << 8) | Uint8(sumb);
		}
	}



	SDL_UpdateTexture(tempTex, &tempSurf->clip_rect, tempPixels, tempSurf->pitch);
	SDL_SetTextureBlendMode(tempTex, SDL_BLENDMODE_BLEND);

}
// Reset Texture pixels with the original pixels from the image
///////////////////////////////////////////////////////////////
void TextureFilter::resetPixels(int id)
{
	m_textureMap[id]->copyPixelData();
	SDL_Texture* tempTex = m_textureMap[id]->getTexture();
	SDL_Surface * tempSurf = m_textureMap[id]->getSurface();

	SDL_UpdateTexture(tempTex, &tempSurf->clip_rect, tempSurf->pixels, tempSurf->pitch);
	SDL_SetTextureBlendMode(tempTex, SDL_BLENDMODE_BLEND);
}

void TextureFilter::calcGaussianKernel(int kernelSize, float sigma)
{
	double sumTotal = 0;
	int kernelRadius = kernelSize / 2;
	double distance = 0;

	double calculatedEuler = 1.0 /(2.0 * M_PI * pow(sigma, 2));

	for (int filterY = -kernelRadius; filterY <= kernelRadius; filterY++)
	{
		for (int filterX = -kernelRadius; filterX <= kernelRadius; filterX++)
		{
			distance = ((filterX * filterX) +(filterY * filterY)) / (2 * (sigma * sigma));
			m_gaussianKernel[filterY + kernelRadius][filterX + kernelRadius] = calculatedEuler * exp(-distance);
			sumTotal += m_gaussianKernel[filterY + kernelRadius][filterX + kernelRadius];
		}
	}

	for (int y = 0; y < kernelSize; y++)
	{
		for (int x = 0; x < kernelSize; x++)
		{
			// Normalise Kernel
			m_gaussianKernel[y][x] = m_gaussianKernel[y][x] * (1.0 / sumTotal);
		}
	}
}
