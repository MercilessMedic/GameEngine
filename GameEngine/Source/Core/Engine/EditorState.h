#pragma once
#include "Scene/EntityManager.h"

const Entity INVALID_ENTITY = 0;

struct EditorState
{
	Entity currentEntity = INVALID_ENTITY;
	glm::vec3 position = { 0.0, 0.0, 0.0};
};