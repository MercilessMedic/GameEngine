#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct TransformComponent
{
	
    glm::vec3 position{0.0f};
	glm::vec3 rotation{0.0f};
	glm::vec3 scale{1.0f};
	glm::mat4 modelMatrix{1.0};
	
    //Forward vector
    glm::vec3 Forward() const
    {
        glm::vec3 dir;
        dir.x = cos(rotation.y) * cos(rotation.x);
        dir.y = sin(rotation.x);
        dir.z = sin(rotation.y) * cos(rotation.x);
        return glm::normalize(dir);
    }
    //Right vector
    glm::vec3 Right() const{ return glm::normalize( glm::cross(Forward(), glm::vec3(0, 1, 0)) ); }
    //Up vector
    glm::vec3 Up() const { return glm::normalize( glm::cross(Right(), Forward()) ); }

};