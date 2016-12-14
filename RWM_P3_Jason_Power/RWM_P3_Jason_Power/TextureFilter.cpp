#include "stdafx.h"
#include "TextureFilter.h"


TextureFilter::TextureFilter()
	: m_gaussianKernel(MAX_KERNAL_SIZE, std::vector<float>(MAX_KERNAL_SIZE))
	, m_gaussianKernel1D(MAX_KERNAL_SIZE)
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

	int emboss_filter[3][3] = { { 0,0,0 },{ 0,1,0 },{ 0,0,-1 } };
	//int edge_filter[3][3] = { { -1,-1,-1 },{ -1,8,-1 },{ -1,-1,-1 } };

	int imageWidth = tempSurf->pitch * 0.25f;
	int imageHeight = tempSurf->h;

	for (int x = 1; x < imageWidth - 1; x++)
	{
		for (int y = 1; y < imageHeight - 1; y++)
		{
			float sumr = 0;
			float sumb = 0;
			float sumg = 0;

			for (int k = -1; k < 2; k++)
			{
				for (int l = -1; l < 2; l++)
				{
					int pos = ((y + l) *imageWidth) + (x + k);
					r = tempPixels[pos] >> 16 & 0xFF;
					g = tempPixels[pos] >> 8 & 0xFF;
					b = tempPixels[pos] & 0xFF;

					sumr += r*emboss_filter[k + 1][l + 1];
					sumg += g*emboss_filter[k + 1][l + 1];
					sumb += b*emboss_filter[k + 1][l + 1];
				}
			}

			if (sumr < 0) sumr = 0;
			if (sumb < 0) sumb = 0;
			if (sumg < 0) sumg = 0;

			if (sumr > 255) sumr = 255;
			if (sumb > 255) sumb = 255;
			if (sumg > 255) sumg = 255;

			tempPixels[y * imageWidth + x] = (0xFF << 24) | (Uint8(sumr) << 16) | (Uint8(sumg) << 8) | Uint8(sumb);
		}
	}

	SDL_UpdateTexture(tempTex, &tempSurf->clip_rect, tempPixels, tempSurf->pitch);
	SDL_SetTextureBlendMode(tempTex, SDL_BLENDMODE_BLEND);

}
void TextureFilter::pixelateFilter(int id)
{
	int blocksize = 5;
	SDL_Texture* tempTex = m_textureMap[id]->getTexture();
	SDL_Surface * tempSurf = m_textureMap[id]->getSurface();
	Uint32* tempPixels = (Uint32*)m_textureMap[id]->getPixels();

	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;

	int imageWidth = tempSurf->pitch * 0.25f;
	int imageHeight = tempSurf->h;

	for (int x = 0; x < imageWidth; x+= blocksize)
	{
		for (int y = 0; y < imageHeight; y+= blocksize)
		{
			float sumA = 0, sumR = 0, sumG = 0, sumB = 0, sumNum = 0;

			for (int xx = 0; xx < blocksize; ++xx)
			{
				for (int yy = 0; yy < blocksize;++yy)
				{
					if (x + xx >= imageWidth || y + yy >= imageHeight)
					{
						continue;
					}
					int pos = ((y + yy) *imageWidth) + x + xx;
					Uint8 a = tempPixels[pos] >> 24 & 0xFF;
					Uint8 r = tempPixels[pos] >> 16 & 0xFF;
					Uint8 g = tempPixels[pos] >> 8 & 0xFF;
					Uint8 b = tempPixels[pos] & 0xFF;
					sumR += r;
					sumG += g;
					sumB += b;
					sumA += a;
					sumNum++;
				}
			}
			sumR /= sumNum;
			sumG /= sumNum;
			sumB /= sumNum;
			sumA /= sumNum;

			for (int j = x; j < x + blocksize; j++)
			{
				for (int k = y; k < y + blocksize; k++)
				{
					tempPixels[ (k * imageWidth) + j ] = Uint8(sumA) << 24 | Uint8(sumR) << 16 | Uint8(sumG) << 8 | Uint8(sumB);
				}
			}
		}
	}
	// Update Textures pixels with the new pixels
	SDL_UpdateTexture(tempTex, &tempSurf->clip_rect, tempPixels, tempSurf->pitch);
	SDL_SetTextureBlendMode(tempTex, SDL_BLENDMODE_BLEND);
}
// Gaussian Blur function - OPTIMIZED
// Blur is applied twice once horizontally and once vertically
///////////////////////////////////////////////////////////////
void TextureFilter::gaussianBlur1D(int id, int kernelSize, float sigma)
{
	calc1DGaussianKernel(kernelSize, sigma);						// Calculate 1D Gaussian Kernel
	SDL_Texture* tempTex = m_textureMap[id]->getTexture();			// Get the Texture from JTexture
	SDL_Surface * tempSurf = m_textureMap[id]->getSurface();		// Get the Surface from JTexture
	Uint32* tempPixels = (Uint32*)m_textureMap[id]->getPixels();	// Get the Pixels from JTexture
	int imageWidth = tempSurf->pitch * 0.25f;						// Get the image width
	int imageHeight = tempSurf->h;									// Get the image height
	int offset = kernelSize * 0.5f;									// Number of elements either side of the kernels middle element

	for (int x = offset; x < imageWidth - offset; x++)
	{
		for (int y = offset; y < imageHeight - offset; y++)
		{
			double sumR = 0;
			double sumB = 0;
			double sumG = 0;

			for (int i = -offset; i <= offset; i++)
			{
				int pos = ((y + i) *imageWidth) + x;
				Uint8 r = tempPixels[pos] >> 16 & 0xFF;
				Uint8 g = tempPixels[pos] >> 8 & 0xFF;
				Uint8 b = tempPixels[pos] & 0xFF;
				sumR += r * m_kernelData[i + offset];
				sumG += g * m_kernelData[i + offset];
				sumB += b * m_kernelData[i + offset];
			}
			tempPixels[y * imageWidth + x] = 0xFF << 24 | Uint8(sumR) << 16 | Uint8(sumG) << 8 | Uint8(sumB);
		}
	}
	for (int x = offset; x < imageWidth - offset; x++)
	{
		for (int y = offset; y < imageHeight - offset; y++)
		{
			double sumR = 0;
			double sumB = 0;
			double sumG = 0;

			for (int i = -offset; i <= offset; i++)
			{
				int pos = (y *imageWidth) + x + i;
				Uint8 r = tempPixels[pos] >> 16 & 0xFF;
				Uint8 g = tempPixels[pos] >> 8 & 0xFF;
				Uint8 b = tempPixels[pos] & 0xFF;
				sumR += r * m_kernelData[i + offset];
				sumG += g * m_kernelData[i + offset];
				sumB += b * m_kernelData[i + offset];
			}
			tempPixels[y * imageWidth + x] = (0xFF << 24) | (Uint8(sumR) << 16) | (Uint8(sumG) << 8) | Uint8(sumB);
		}
	}
	// Update Textures pixels with the new pixels
	SDL_UpdateTexture(tempTex, &tempSurf->clip_rect, tempPixels, tempSurf->pitch);
	SDL_SetTextureBlendMode(tempTex, SDL_BLENDMODE_BLEND);

	// Release Memory
	delete[] m_kernelData;
	m_kernelData = nullptr;
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

// Calculate 1D Gaussian Kernel / Convolution Matrix
///////////////////////////////////////////////////////////////
void TextureFilter::calc1DGaussianKernel(int kernelSize, float sigma)
{
	m_kernelData = new double[kernelSize];
	double sum = 0;
	double calculatedEuler = 1.0 / (2.0 * M_PI * pow(sigma, 2));
	double coeff = (2 * (sigma * sigma));
	int kernelRadius = kernelSize / 2;
	for (int i = -kernelRadius; i <= kernelRadius; i++)
	{
		m_kernelData[i + kernelRadius] = calculatedEuler * exp(-i * i / coeff);
		sum += m_kernelData[i + kernelRadius];
	}
	for (int i = 0; i < kernelSize; i++)
	{
		m_kernelData[i] = m_kernelData[i] / sum;
	}
	for (int i = 0; i < kernelSize; i++)
	{
		std::cout << m_kernelData[i] << std::endl;
	}
}