#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

#include <windows.h>
#include <iostream>


struct Screen 
{
	int WIDTH, HEIGHT;
};

class Globals 
{
public:
	Screen getScreenSize(HWND hwnd);


};





#endif