#include "Mesh.h"
#include <vector>


void Mesh::start(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	IndexedModel model;

	for (unsigned int i = 0; i < numVertices; i++)
	{
		model._positions.push_back(*vertices[i].GetPosition());
		model._textureCoords.push_back(*vertices[i].GetTextureCoord());
		model._normals.push_back(*vertices[i].GetNormal());
	}

	for (unsigned int i = 0; i < numIndices; i++)
		model._indices.push_back(indices[i]);

	startModel(model);
}

void Mesh::startModel(const IndexedModel& model)
{

	drawObjectCount = model._indices.size();

	glGenVertexArrays(1, &vertexArrayObj); //generate a vertex array and store it in the VAO
	glBindVertexArray(vertexArrayObj); //bind the VAO (any operation that works on a VAO will work on our bound VAO - binding)

	glGenBuffers(NUM_BUFFERS, vertexArrayBuff); //generate our buffers based of our array of data/buffers - GLuint vertexArrayBuffers[NUM_BUFFERS];

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuff[POSITION_VERTEXBUFFER]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ARRAY_BUFFER, model._positions.size() * sizeof(model._positions[0]), &model._positions[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU (determined by type)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuff[TEXCOORD_VB]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ARRAY_BUFFER, model._positions.size() * sizeof(model._textureCoords[0]), &model._textureCoords[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuff[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model._normals[0]) * model._normals.size(), &model._normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuff[INDEX_VB]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model._indices.size() * sizeof(model._indices[0]), &model._indices[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU

	glBindVertexArray(0); // unbind our VAO
}

Mesh::Mesh()
{
	drawObjectCount = NULL;
}

void Mesh::loadingModel(const std::string& filename)
{
	IndexedModel model = ObjectModel(filename).ToIndexedModel();
	startModel(model);
	Sphere sphereMesh();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vertexArrayObj); // delete arrays
}

void Mesh::drawing()
{
	glBindVertexArray(vertexArrayObj);

	glDrawElements(GL_TRIANGLES, drawObjectCount, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, drawCount);

	glBindVertexArray(0);
}

void Mesh::updateSphereData(glm::vec3 pos, float radius)
{
	sphereMesh.SetPos(pos);
	sphereMesh.SettingRadius(radius);
}

