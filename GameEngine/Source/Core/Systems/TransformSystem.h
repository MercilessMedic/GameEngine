#pragma once

#include "Scene/EntityManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class TransformSystem
{
public:
	void update(EntityManager& em)
	{
		for(auto it = em.transforms.begin(); it != em.transforms.end(); it++)
		{
			
			TransformComponent& transform = it->second;
			transform.modelMatrix = computeLocalMatrix( transform );
		}
	}
private:
		glm::mat4 computeLocalMatrix(TransformComponent transform) const
		{
			//Identity matrix
			glm::mat4 I = glm::mat4(1.0);

			//Coordinate basis vectors
			glm::vec3 X = glm::vec3(1.0, 0.0, 0.0);
			glm::vec3 Y = glm::vec3(0.0, 1.0, 0.0);
			glm::vec3 Z = glm::vec3(0.0, 0.0, 1.0);

			//Translation matrix
			glm::mat4 T = glm::translate(I, transform.position);

			//Rotation matrices
			glm::mat4 Rx = glm::rotate(I, glm::radians(transform.rotation.x), X);
			glm::mat4 Ry = glm::rotate(I, glm::radians(transform.rotation.y), Y);
			glm::mat4 Rz = glm::rotate(I, glm::radians(transform.rotation.z), Z);

			//For Euler angles first we have Yaw, then Pitch  then Roll 
			glm::mat4 R = Ry * Rx * Rz;

			//Scale matrix
			glm::mat4 S = glm::scale(I, transform.scale);

			//First is translation then rotation then scaling
			return T * R * S;
		}
};