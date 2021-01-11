#pragma once

#include <string>

class GBEBaseWindow;

#include <Windows.h>


enum GBEWindowStyle
{
	GBEWindowCanClose = 0x1,
	GBEWindowCanResize = 0x2,
	GBEWindowHaveTitlebar = 0x4,
	GBEWindowCentered = 0x8,
	GBEWindowFullscreen = 0x10,

	GBEWindowDefault = GBEWindowCanClose | GBEWindowCanResize | GBEWindowHaveTitlebar | GBEWindowCentered,
	GBEWindowDefaultNoResize = GBEWindowCanClose | GBEWindowHaveTitlebar | GBEWindowCentered
};

struct GBEWin32VulkanConfig
{
	GBEBaseWindow *window;
	uint16_t	vkMajVersion;
	uint16_t	vkMinVersion;
	uint8_t		vkShaderVersionMaj;
	uint8_t		vkShaderVersionMin;

	bool		supportDrawTexture;
	bool		supportTextureRect;
	bool		supportOcclusionQuery;
	bool		supportDDS;

	uint32_t	maxTextureSize;
	int32_t		zbufferBitDepth;
};

struct GBEngineConfig
{
	std::string		windowTitle = "GBEngine";
	bool			runInWindow = true;
	uint16_t		windowWidth = 800;
	uint16_t		windowHeight = 600;
	uint32_t		windowFlags = 0;
};

struct GBERect
{
	int32_t	x=0;
	int32_t	y=0;
	int32_t	width=0;
	int32_t	height=0;

	GBERect() {};

	GBERect(int32_t w, int32_t h)
	{
		x = y = 0;
		width = w;
		height = h;
	}
	GBERect(int32_t x, int32_t y, int32_t w, int32_t h)
	{
		this->x = x;
		this->y = y;
		width = w;
		height = h;
	}
};

using GBEWINDOWHANDLE = HWND;
using GBEPlatformConfig = GBEWin32VulkanConfig;
