#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>
using namespace std;


class ScreenDisplay
{
public:
	ScreenDisplay();
	~ScreenDisplay();
	void startingDisplay();
	void swappingBuff();
	void clearingDisplay(float r, float g, float b, float a);

	float getScreenWidth();
	float getScreenHeight();

private:

	void returnError(std::string errorString);
	
	SDL_GLContext gl_Context; //global variable to hold the context
	SDL_Window* sdl_Window; //holds pointer to out window
	float width;
	float height;
};

