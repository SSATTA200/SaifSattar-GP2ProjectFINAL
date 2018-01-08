#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "ScreenDisplay.h" 
#include "Shaders.h"
#include "Mesh.h"
#include "Textures.h"
#include "TransformPos.h"
#include "AudioSound.h"

enum class GameState{PLAY, EXIT};

class Game
{
public:
	Game();
	~Game();

	void runGame();

private:

	void startSystems();
	void processInputs();
	void mainGameLoop();
	void drawingGame();
	bool colliding(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad, glm::vec3 m3Pos, float m3Rad);
	void playAudio(unsigned int Source, glm::vec3 pos);

	ScreenDisplay g_Display;
	GameState g_State;
	Mesh mesh1;
	Mesh mesh2;
	Mesh mesh3;
	Mesh mesh4;
	Mesh mesh5;
	Cam myCam;
	AudioSound aud_Device;
	float ship_x = 0.0, ship_y = 0.0;
	float ship_xvel = 0.0, ship_yvel = 0.0;

	float num_Counter;
	unsigned int bang;
	unsigned int music;
};

