#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Cam.h"

struct Transform
{
public:
	Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f))
	{
		this->position = pos;
		this->rotation = rot;
		this->scale = scale;
	}

	inline glm::mat4 GetModel() const
	{
		glm::mat4 posMat = glm::translate(position);
		glm::mat4 scaleMat = glm::scale(scale);
		glm::mat4 rotX = glm::rotate(rotation.x, glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 rotY = glm::rotate(rotation.y, glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 rotZ = glm::rotate(rotation.z, glm::vec3(0.0, 0.0, 1.0));
		glm::mat4 rotMat = rotX * rotY * rotZ;

		return posMat * rotMat * scaleMat;
	}

	/*inline glm::mat4 GetMVP(const Camera& camera) const
	{
		glm::mat4 VP = camera.GetViewProjection();
		glm::mat4 M = GetModel();

		return VP * M;//camera.GetViewProjection() * GetModel();
	}*/

	inline glm::vec3* GetPosition() { return &position; } //getters
	inline glm::vec3* GetRotation() { return &rotation; }
	inline glm::vec3* GetScale() { return &scale; }

	inline void SetPosition(glm::vec3& pos) { this->position = pos; } // setters
	inline void SetRotation(glm::vec3& rot) { this->rotation = rot; }
	inline void SetScale(glm::vec3& scale) { this->scale = scale; }
protected:
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};


