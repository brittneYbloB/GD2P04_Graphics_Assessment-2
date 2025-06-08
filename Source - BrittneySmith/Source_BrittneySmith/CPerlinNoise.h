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

#pragma once
// Library Includes
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

static const int g_Width = 512;
static const int g_Height = 512;
static const int g_ChannelNum = 4;
static const int g_PixelAmount = g_Width * g_Height;

class CPerlinNoise
{
public:
	CPerlinNoise();
	~CPerlinNoise() {};

	double RandomValue(int _X, int _Y);
	double Smooth(int _X, int _Y);
	double LinearInterpolate(double _Point1, double _Point2, double _Fract);
	double CosineInterpolate(double _Point1, double _Point2, double _Fract);
	double SmoothInterpolate(double _X, double _Y);
	double TotalNoisePerPoint(int _X, int _Y);
	void SaveToFile();

public:
	int m_Seed = 0;
};

