#pragma once
#include <string>
#include <GL\glew.h>
#include "TransformPos.h"
#include "Cam.h"

class Shaders
{
public:
	Shaders(const std::string& filename);

	void Bind(); //Set gpu to use our shaders
	void Update(const Transform& transform, const Cam & camera);

	std::string Shaders::LoadingShader(const std::string& fileName);
	void Shaders::CheckingShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint Shaders::CreatingShader(const std::string& text, unsigned int type);

    ~Shaders();


protected:
private:
	static const unsigned int NUM_SHADERS = 2; // number of shaders

	enum
	{
		TRANSFORM_U,

		NUM_UNIFORMS
	};

	GLuint _program; // Track the shader program
	GLuint _shaders[NUM_SHADERS]; //array of shaders
	GLuint _uniforms[NUM_UNIFORMS]; //no of uniform variables
};
