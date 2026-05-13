#pragma once
#include "Renderer/Model.h"

struct ModelComponent
{
	std::shared_ptr<Model> model;
};

struct MeshComponent
{
	std::shared_ptr<Mesh> mesh;
};