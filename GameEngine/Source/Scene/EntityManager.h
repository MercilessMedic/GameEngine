#pragma once

#include <iostream>
#include <unordered_map>
#include "TransformComponent.h"
#include "RenderComponents.h"

//This is just a handle for an Entity in the game world
typedef uint32_t Entity;

class EntityManager
{
public:
		
	int numEntities = 0;

	std::unordered_map<Entity, TransformComponent> transforms;
	std::unordered_map<Entity, ModelComponent> modelComponents;
	//Mesh components are specifically for Entities that don't have high level models
	std::unordered_map<Entity, MeshComponent> meshComponents;

	Entity createEntity() 
	{
		numEntities += 1;
		addTransformComponent(numEntities, TransformComponent());
		return numEntities;
	}

	void destroyEntity(Entity e)
	{
		transforms.erase(e);
		modelComponents.erase(e);
		meshComponents.erase(e);
	}

	void addTransformComponent( Entity e, TransformComponent transform) 
	{
		transforms[ e ] = transform;
	}

	void addModelComponent( Entity e, ModelComponent modelComp )
	{
		modelComponents[ e ] = modelComp;
	}

	void addMeshComponent( Entity e, MeshComponent renderComponent )
	{
		meshComponents[ e ] = renderComponent;
	}

	bool hasTransform( Entity e)
	{
		return transforms.find(e) != transforms.end();
	}
	
	bool hasModel( Entity e )
	{
		return modelComponents.find(e) != modelComponents.end();
	}

	bool hasMesh( Entity e )
	{
		return meshComponents.find(e) != meshComponents.end();
	}
	
	TransformComponent* getTransformComponent( Entity e )
	{
		auto it = transforms.find(e);
		if( it == transforms.end())
		{
			return nullptr;
		}
		return &it->second;
	}

	ModelComponent* getModelComponent( Entity e )
	{
		auto it = modelComponents.find( e );
		if( it == modelComponents.end() )
		{
			return nullptr;
		}
		return &it->second;
	}
	
	MeshComponent* getMeshComponent(Entity e)
	{
		auto it = meshComponents.find(e);
		if (it == meshComponents.end())
		{
			return nullptr;
		}
		return &it->second;
	}

};