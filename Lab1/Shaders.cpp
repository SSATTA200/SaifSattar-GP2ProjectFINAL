#include "Shaders.h"
#include <iostream>
#include <fstream>

Shaders::Shaders(const std::string& filename)
{
	_program = glCreateProgram(); // create shader program (openGL saves as ref number)
	_shaders[0] = CreatingShader(LoadingShader("..\\res\\shader.vert"), GL_VERTEX_SHADER); // create vertex shader
	_shaders[1] = CreatingShader(LoadingShader("..\\res\\shader.frag"), GL_FRAGMENT_SHADER); // create fragment shader

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glAttachShader(_program, _shaders[i]); //add all our shaders to the shader program "shaders" 
	}

	glBindAttribLocation(_program, 0, "position"); // associate attribute variable with our shader program attribute (in this case attribute vec3 position;)
	glBindAttribLocation(_program, 1, "texCoord"); 

	glLinkProgram(_program); //create executables that will run on the GPU shaders
	CheckingShaderError(_program, GL_LINK_STATUS, true, "Error: Shader program linking failed"); // cheack for error

	glValidateProgram(_program); //check the entire program is valid
	CheckingShaderError(_program, GL_VALIDATE_STATUS, true, "Error: Shader program not valid");

	_uniforms[TRANSFORM_U] = glGetUniformLocation(_program, "transform"); // associate with the location of uniform variable within a program
}


Shaders::~Shaders()
{
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(_program, _shaders[i]); //detach shader from program
		glDeleteShader(_shaders[i]); //delete the sahders
	}
	glDeleteProgram(_program); // delete the program
}

void Shaders::Bind()
{
	glUseProgram(_program); //installs the program object specified by program as part of rendering state
}

void Shaders::Update(const Transform& transform, const Cam& camera)
{
	glm::mat4 mvp = camera.GetViewProject() * transform.GetModel();
	glUniformMatrix4fv(_uniforms[TRANSFORM_U], 1, GLU_FALSE, &mvp[0][0]);
}


GLuint Shaders::CreatingShader(const std::string& text, unsigned int type)
{
	GLuint _shader = glCreateShader(type); //create shader based on specified type

	if (_shader == 0) //if == 0 shader no created
		std::cerr << "Error type creation failed " << type << std::endl;

	const GLchar* stringSource[1]; //convert strings into list of c-strings
	stringSource[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(_shader, 1, stringSource, lengths); //send source code to opengl
	glCompileShader(_shader); //get open gl to compile shader code

	CheckingShaderError(_shader, GL_COMPILE_STATUS, false, "Error compiling shader!"); //check for compile error

	return _shader;
}

std::string Shaders::LoadingShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void Shaders::CheckingShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

