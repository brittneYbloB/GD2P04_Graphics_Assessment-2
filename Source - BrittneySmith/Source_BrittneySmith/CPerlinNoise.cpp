/***********************************************************************
Bachelor of Software Engineering
Media Design School
Auckland
New Zealand
(c) [2022] Media Design School
File Name :
Description : GD2P04 : 3D Graphics Programming
Author : Brittney Smith
Mail : Brittney.Smith@mds.ac.nz
**************************************************************************/

#include "CPerlinNoise.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION  
#include "stb_image_write.h"

/***************************************************************************
* CPerlinNoise: Constructor
* @parameter:
* @return:
****************************************************************************/
CPerlinNoise::CPerlinNoise()
{
}

/***************************************************************************
* RandomValue:
* @parameter:
* @return:
****************************************************************************/
double CPerlinNoise::RandomValue(int _X, int _Y)
{
	double randomValue;
	int Noise = _X + _Y * m_Seed;		// All calls within a single texture generation use the same seed
	Noise = (Noise << 13) ^ Noise;

	int T = (Noise * (Noise * Noise * 15731 + 789221) + 1376312589) & 0x7fffffff;
	randomValue = 1.0f - (double)T * 0.93132257461548515625e-9;

	return randomValue;
}

/***************************************************************************
* Smooth:
* @parameter:
* @return:
****************************************************************************/
double CPerlinNoise::Smooth(int _X, int _Y)
{
	double Corners = (RandomValue(_X - 1, _Y - 1) + RandomValue(_X + 1, _Y - 1) + RandomValue(_X - 1, _Y + 1) + RandomValue(_X + 1, _Y + 1)) / 16.0f;
	double Sides = (RandomValue(_X - 1, _Y) + RandomValue(_X + 1, _Y) + RandomValue(_X, _Y - 1) + RandomValue(_X, _Y + 1)) / 8.0f;
	double Center = (RandomValue(_X, _Y)) / 4.0f;	// the divisors are used to control weight and impact to current point

	return Corners + Sides + Center;
}

/***************************************************************************
* LinearInterpolate:
* @parameter:
* @return:
****************************************************************************/
double CPerlinNoise::LinearInterpolate(double _Point1, double _Point2, double _Fract)
{
	return (_Point1 * (1 - _Fract) + _Point2 * _Fract);
}

/***************************************************************************
* CosineInterpolate:
* @parameter:
* @return:
****************************************************************************/
double CPerlinNoise::CosineInterpolate(double _Point1, double _Point2, double _Fract)
{
	float PI = 3.14159f;
	double Fract2 = (1 - cos(_Fract * PI)) / 2;
	return (_Point1 * (1 - Fract2) + _Point2 * Fract2);
}

/***************************************************************************
* SmoothInterpolate:
* @parameter:
* @return:
****************************************************************************/
double CPerlinNoise::SmoothInterpolate(double _X, double _Y)
{
	int TruncatedX = (int)_X;
	int TruncatedY = (int)_Y;

	// Get the fraction component of X and Y (only the value after the decimal)
	double FractX = _X - (double)TruncatedX;
	double FractY = _Y - (double)TruncatedY;

	// Smoothing
	double V1 = Smooth(TruncatedX, TruncatedY);
	double V2 = Smooth(TruncatedX + 1, TruncatedY);
	double V3 = Smooth(TruncatedX, TruncatedY + 1);
	double V4 = Smooth(TruncatedX + 1, TruncatedY + 1);

	// Interpolates
	double Interpolate_1 = CosineInterpolate(V1, V2, (double)FractX);
	double Interpolate_2 = CosineInterpolate(V3, V4, (double)FractX);

	double Final = CosineInterpolate(Interpolate_1, Interpolate_2, FractY);	// Bilinear interpolation
	return Final;
}

/***************************************************************************
* TotalNoisePerPoint: 
* @parameter:
* @return:
****************************************************************************/
double CPerlinNoise::TotalNoisePerPoint(int _X, int _Y)
{
	int Octaves = 6;
	float Wavelength = 128.0f;
	float Gain = 0.5f;
	float Lacunarity = 2.0f;

	float MaxValue = 0.0f;
	double Total = 0.0f;

	for (int i = 0; i < Octaves; i++)
	{
		float Frequency = (float)pow(Lacunarity, i) / Wavelength;
		float Amplitude = (float)pow(Gain, i);
		MaxValue += Amplitude;

		Total += SmoothInterpolate(_X * Frequency, _Y * Frequency) * Amplitude;
	}

	return (Total / MaxValue);
}

/***************************************************************************
* SaveToFile: Save to .raw file
* @parameter:
* @return:
****************************************************************************/
void CPerlinNoise::SaveToFile()
{
	// Variables for converting [-1 -> 1] to [0 -> 255]
	double OldMin = -1;
	double OldMax = 1;
	double NewMin = 0;
	double NewMax = 255;

	double OldRange = (OldMax - OldMin);
	double NewRange = (NewMax - NewMin);

	// Pixel array (size = width * depth)
	uint8_t* Pixels = new uint8_t[g_PixelAmount];

	int i = 0;	// incremented in for loops; used to ensure entire array is initialised to a value
	// Call TotalNoisePerPoint(x, y) for each pixel of texture image
	for (int x = 0; x < g_Width; x++)
	{
		for (int y = 0; y < g_Height; y++)
		{
			// Before 8-bit conversion
			double OldValue = TotalNoisePerPoint(x, y);
			// After 8-bit conversion
			Pixels[i] = int((((OldValue - OldMin) * NewRange) / OldRange) + NewMin);

			// Increment pixel element in array
			i += 1;
		}
	}

	// Save as .raw file
	std::string FileName = "PerlinNoise";
	std::ofstream RawFile("Images/" + FileName + ".raw", std::ios_base::binary);
	if (RawFile)
	{
		char* c = (char*)&Pixels[0];
		RawFile.write((char*)&Pixels[0], (std::streamsize)((int)g_Width * (int)g_Height));
		RawFile.close();
	}

	// Colour gradient
	uint8_t* PixelsRGBA = new uint8_t[g_PixelAmount * g_ChannelNum];
	int j = 0;	
	int k = 0;	
	for (int x = 0; x < g_Width; x++)
	{
		for (int y = 0; y < g_Height; y++)
		{
			// BLUE
			if (Pixels[k] <= 115)
			{
				PixelsRGBA[j]		= 0;	// R
				PixelsRGBA[j+1]		= 0;	// G
				PixelsRGBA[j+2]		= 255;	// B
				PixelsRGBA[j+3]		= 1;	// A
			}
			// GREEN
			else if ((Pixels[k] < 125 && Pixels[k] > 115) || Pixels[k] == 125)
			{
				PixelsRGBA[j]	  = 0;
				PixelsRGBA[j + 1] = 255;
				PixelsRGBA[j + 2] = 0;
				PixelsRGBA[j + 3] = 1;
			}
			// YELLOW
			else if ((Pixels[k] < 135 && Pixels[k] > 125) || Pixels[k] == 135)
			{
				PixelsRGBA[j]		= 255;
				PixelsRGBA[j+1]		= 255;
				PixelsRGBA[j+2]		= 0;
				PixelsRGBA[j+3]		= 1;
			}
			// ORANGE
			else if ((Pixels[k] < 145 && Pixels[k] > 135) || Pixels[k] == 145)
			{
				PixelsRGBA[j]	  = 200;
				PixelsRGBA[j + 1] = 128;
				PixelsRGBA[j + 2] = 0;
				PixelsRGBA[j + 3] = 1;
			}
			// PURPLE
			else if ((Pixels[k] < 150 && Pixels[k] > 145) || Pixels[k] == 150)
			{
				PixelsRGBA[j]	  = 255;
				PixelsRGBA[j + 1] = 0;
				PixelsRGBA[j + 2] = 255;
				PixelsRGBA[j + 3] = 1;
			}
			// RED
			else if ((Pixels[k] < 160 && Pixels[k] > 150) || Pixels[k] == 160)
			{
				PixelsRGBA[j]		= 255;
				PixelsRGBA[j+1]		= 0;
				PixelsRGBA[j+2]		= 0;
				PixelsRGBA[j+3]		= 1;
			}

			// Increment pixel element in array
			j += 4;
			k += 1;
		}
	}

	// Save as .jpg file
	std::string textureName = "PerlinNoiseTexture";
	stbi_write_jpg(("Images/" + textureName + ".jpg").c_str(), g_Width, g_Height, g_ChannelNum, PixelsRGBA, 100);
}
