#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct Cam
{
public:
	Cam()
	{
	}

	void startCam(const glm::vec3& _pos, float _fov, float _aspect, float _near_Clip, float _far_Clip)
	{
		this->cam_position = _pos;
		this->cam_forward = glm::vec3(0.0f, 0.0f, 1.0f);
		this->cam_up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->cam_project = glm::perspective(_fov, _aspect, _near_Clip, _far_Clip);
	}

	glm::vec3 getPosition()
	{
		return this->cam_position;
	}

	inline glm::mat4 GetViewProject() const
	{
		return cam_project * glm::lookAt(cam_position, cam_position + cam_forward, cam_up);
	}

	//void MoveForward(float amt)
	//{
	//	pos += forward * amt;
	//}

	//void MoveRight(float amt)
	//{
	//	pos += glm::cross(up, forward) * amt;
	//}

	//void Pitch(float angle)
	//{
	//	glm::vec3 right = glm::normalize(glm::cross(up, forward));

	//	forward = glm::vec3(glm::normalize(glm::rotate(angle, right) * glm::vec4(forward, 0.0)));
	//	up = glm::normalize(glm::cross(forward, right));
	//}

	//void RotateY(float angle)
	//{
	//	static const glm::vec3 UP(0.0f, 1.0f, 0.0f);

	//	glm::mat4 rotation = glm::rotate(angle, UP);

	//	forward = glm::vec3(glm::normalize(rotation * glm::vec4(forward, 0.0)));
	//	up = glm::vec3(glm::normalize(rotation * glm::vec4(up, 0.0)));
	//}

protected:
private:
	glm::mat4 cam_project;
	glm::vec3 cam_position;
	glm::vec3 cam_forward;
	glm::vec3 cam_up;
};


