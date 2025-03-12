#include "Globals.h"


int Globals::getWindowHeight(HWND hwnd)
{
	RECT rect;

	if (GetClientRect(hwnd, &rect))
	{
		int height = rect.bottom - rect.top;

		return height;
	}

}


int Globals::getWindowWidth(HWND hwnd)
{
	RECT rect;

	if (GetClientRect(hwnd, &rect))
	{
		int width = rect.right - rect.left;

		return width;
	}

}