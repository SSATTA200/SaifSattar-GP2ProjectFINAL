#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <string>
#include "LoadObject.h"

struct Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord)
	{
		this->_position = _position;
		this->_texture_Coord = _texture_Coord;
		this->_normal = _normal;
	}

	glm::vec3* GetPosition() { return &_position; }
	glm::vec2* GetTextureCoord() { return &_texture_Coord; }
	glm::vec3* GetNormal() { return &_normal; }

private:
	glm::vec3 _position;
	glm::vec2 _texture_Coord;
	glm::vec3 _normal;
};

struct Sphere
{
public:

	Sphere() {}

	Sphere(glm::vec3& position, float radius)
	{
		this->position = position;
	}

	glm::vec3 GetPosition() { return position; }
	float GetRadius() { return radius; }

	void SetPos(glm::vec3 pos)
	{
		this->position = pos;
	}

	void SettingRadius(float radius)
	{
		this->radius = radius;
	}

private:
	glm::vec3 position;
	float radius;
};

class Mesh
{
public:
	Mesh();
	~Mesh();


	void drawing();
	void start(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	void loadingModel(const std::string& filename);
	void startModel(const IndexedModel& model);
	void updateSphereData(glm::vec3 pos, float radius);
	glm::vec3 getSpherePos() { return sphereMesh.GetPosition(); }
	float getSphereRadius() { return sphereMesh.GetRadius(); }

private:



	enum
	{
		POSITION_VERTEXBUFFER,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	Sphere sphereMesh;
	GLuint vertexArrayObj;
	GLuint vertexArrayBuff[NUM_BUFFERS]; // create our array of buffers
	unsigned int drawObjectCount; //how much of the vertexArrayObject do we want to draw
};