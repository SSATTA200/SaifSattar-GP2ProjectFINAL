#include "Game.h"
#include "Cam.h"
#include <iostream>
#include <string>
#include <SDL\SDL.h>


Transform transform;

Game::Game()
{
	g_State = GameState::PLAY;
	ScreenDisplay* g_Display = new ScreenDisplay(); //new display
    Mesh* mesh1();
	Mesh* mesh2();
	Mesh* mesh3();
	AudioSound* aud_Device();

	
	

}

Game::~Game()
{
}

void Game::runGame()
{
	startSystems(); 
	mainGameLoop();
}

void Game::startSystems()
{
	g_Display.startingDisplay(); 
	bang = aud_Device.loadAudio("..\\res\\bang.wav");
	music = aud_Device.loadAudio("..\\res\\background.wav");
	
	mesh1.loadingModel("..\\res\\pixelspaceship.obj");
	mesh2.loadingModel("..\\res\\monkey3.obj");
	mesh3.loadingModel("..\\res\\monkey4.obj");
	
	myCam.startCam(glm::vec3(0, 0, -10), 70.0f, (float)g_Display.getScreenWidth()/g_Display.getScreenHeight(), 0.01f, 1000.0f);

	num_Counter = 1.0f;
}

void Game::mainGameLoop()
{
	while (g_State != GameState::EXIT)
	{
		processInputs();
		drawingGame();
		colliding(mesh1.getSpherePos(), mesh1.getSphereRadius(), mesh2.getSpherePos(), mesh2.getSphereRadius(), mesh3.getSpherePos(), mesh3.getSphereRadius());
		playAudio(music, glm::vec3(0.0f,0.0f,0.0f));
	}
}

void Game::processInputs()
{
	SDL_Event event;

	while(SDL_PollEvent(&event)) //get and process events
	{
		switch (event.type)
		{
			case SDL_QUIT:
				g_State = GameState::EXIT;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_LEFT:
						ship_xvel = ship_xvel + 0.2;
						break;

					case SDLK_RIGHT:
						ship_xvel = ship_xvel - 0.2;
						break;

					case SDLK_UP:
						ship_yvel = ship_yvel + 0.2;
						break;
					case SDLK_DOWN:
						ship_yvel = ship_yvel - 0.2;
						break;
					default:
						break;
					
				}
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
					//if (ship_xvel > 0)
						//ship_xvel = 0;
					break;

				case SDLK_RIGHT:
					//if (ship_xvel < 0)
						//ship_xvel = 0;
					break;

				case SDLK_UP:
					//if (ship_yvel > 0)
						//ship_yvel = 0;
					break;
				case SDLK_DOWN:
					//if (ship_yvel < 0)
						//ship_yvel = 0;
					break;
				default:
					break;
				}

			defaut:
				break;
		}
	}
	
}


bool Game::colliding(glm::vec3 m1_Pos, float m1_Rad, glm::vec3 m2_Pos, float m2_Rad, glm::vec3 m3_Pos, float m3_Rad)
{
	float dist = glm::sqrt((m2_Pos.x - m1_Pos.x)*(m2_Pos.x - m1_Pos.x) + (m2_Pos.y - m1_Pos.y)*(m2_Pos.y - m1_Pos.y) + (m2_Pos.z - m1_Pos.z)*(m2_Pos.z - m1_Pos.z));

	if (dist < (m1_Rad + m2_Rad))
	{
		aud_Device.setAudiolistener(myCam.getPosition(), m1_Pos); //add bool to mesh
		playAudio(bang, m1_Pos);
		return true;
	}
	else
	{
		return false;
	}
}

void Game::playAudio(unsigned int Source, glm::vec3 pos)
{
	
	ALint state; 
	alGetSourcei(Source, AL_SOURCE_STATE, &state);
	/*
	Possible values of state
	AL_INITIAL
	AL_STOPPED
	AL_PLAYING
	AL_PAUSED
	*/
	if (AL_PLAYING != state)
	{
		aud_Device.playAudio(Source, pos);
	}
}

void Game::drawingGame()
{
	g_Display.clearingDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	Shaders shader("..\\res\\shader"); //new shader
	Textures texture("..\\res\\bricks.jpg"); //load texture
	Textures texture1("..\\res\\water.jpg"); //load texture
	
	transform.SetPosition(glm::vec3(ship_xvel, ship_yvel, -1.0));
	transform.SetRotation(glm::vec3(0.0, -1.5, num_Counter * 0));
	transform.SetScale(glm::vec3(0.2, 0.2, 0.2));

	shader.Bind();
	shader.Update(transform, myCam);
	texture.Binding(1);
	mesh1.drawing();
	mesh1.updateSphereData(*transform.GetPosition(), 0.62f);

	/////////////////////////////////////////////////////////////////////////////////////////////

	transform.SetPosition(glm::vec3(-sinf(num_Counter), -0.5, -sinf(num_Counter)*5));
	transform.SetRotation(glm::vec3(0.0, 0.0, num_Counter * 5));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	shader.Bind();
	shader.Update(transform, myCam);
	texture.Binding(0);
	mesh2.drawing();
	mesh2.updateSphereData(*transform.GetPosition(), 0.62f);
	num_Counter = num_Counter + 0.05f;

	////////////////////////////////////////////////////////////////////////////////////////////

	transform.SetPosition(glm::vec3(sinf(num_Counter)+2.0, 1.5, sinf(num_Counter) * 5));
	transform.SetRotation(glm::vec3(3.0, 0.0, num_Counter * 3));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	shader.Bind();
	shader.Update(transform, myCam);
	texture.Binding(0);
	mesh3.drawing();
	mesh3.updateSphereData(*transform.GetPosition(), 0.62f);
	num_Counter = num_Counter + 0.05f;

	////////////////////////////////////////////////////////////////////////////////////////////
				
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	g_Display.swappingBuff();
} 