#include "ScreenDisplay.h"


ScreenDisplay::ScreenDisplay()
{
	sdl_Window = nullptr; //initialise to generate null access violation for debugging. 
	width = 1024.0f;
	height = 768.0f; 
}

ScreenDisplay::~ScreenDisplay()
{
	SDL_GL_DeleteContext(gl_Context); // delete context
	SDL_DestroyWindow(sdl_Window); // detete window (make sure to delete the context and the window in the opposite order of creation in initDisplay())
	SDL_Quit();
}

float ScreenDisplay::getScreenWidth() { return width; } //getters
float ScreenDisplay::getScreenHeight() { return height; }

void ScreenDisplay::returnError(std::string errorString)
{
	std::cout << errorString << std::endl;
	std::cout << "press any  key to quit...";
	int in;
	std::cin >> in;
	SDL_Quit();
}

void ScreenDisplay::swappingBuff()
{
	SDL_GL_SwapWindow(sdl_Window); //swap buffers
}

void ScreenDisplay::clearingDisplay(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear colour and depth buffer - set colour to colour defined in glClearColor
}

void ScreenDisplay::startingDisplay()
{
	SDL_Init(SDL_INIT_EVERYTHING); //initalise everything

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); //Min no of bits used to diplay colour
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);// set up z-buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // set up double buffer   

	sdl_Window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)width, (int)height, SDL_WINDOW_OPENGL); // create window

	if (sdl_Window == nullptr)
	{
		returnError("window failed to create");
	}

	gl_Context = SDL_GL_CreateContext(sdl_Window);

	if (gl_Context == nullptr)
	{
		returnError("SDL_GL context failed to create");
	}

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		returnError("GLEW failed to initialise");
	}

	glEnable(GL_DEPTH_TEST); //enable z-buffering 
	glEnable(GL_CULL_FACE); //dont draw faces that are not pointing at the camera

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
}