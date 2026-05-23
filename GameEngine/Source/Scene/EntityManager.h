#pragma once

#include <iostream>
#include <unordered_map>
#include "TransformComponent.h"
#include "RenderComponents.h"
#include "TagComponent.h"
#include <unordered_set>
//This is just a handle for an Entity in the game world
typedef uint32_t Entity;

class EntityManager
{
public:
	

	std::unordered_set<Entity> entities;	
	Entity nextEntity = 0;
	std::unordered_map<Entity, TransformComponent> transforms;
	std::unordered_map<Entity, ModelComponent> modelComponents;
	std::unordered_map<Entity, MeshComponent> meshComponents; //Mesh components are specifically for Entities that don't have high level models
	std::unordered_map<Entity, TagComponent> tagComponents;

	Entity createEntity() 
	{
		nextEntity += 1;
		entities.insert(nextEntity);
		//Automatically create default transform and tag components for newly created entity
		addTransformComponent(nextEntity, TransformComponent{});
		addTagComponent(nextEntity, TagComponent{ "Entity" + to_string(nextEntity) });
		return nextEntity;
	}

	void destroyEntity(Entity e)
	{
		entities.erase(e);
		transforms.erase(e);
		modelComponents.erase(e);
		meshComponents.erase(e);
	}

	bool entityExists( Entity e )
	{
		return entities.find(e) != entities.end();
	}
	
	void addTagComponent(Entity e, TagComponent tag)
	{
		if (entityExists(e))
		{
			tagComponents[e] = tag;
		}
		else
		{
			printf("[WARNING] [EntityManager] can not add tag Component. Entity by Id %d does not exist!", e);
		}
	}

	void addTransformComponent( Entity e, TransformComponent transform) 
	{
		if( entityExists(e) )
		{
			transforms[e] = transform;
		}
		else
		{
			printf("[WARNING] [EntityManager] can not add transform. Entity by Id %d does not exist!", e);
		}
		
	}

	void addModelComponent( Entity e,  ModelComponent modelComp )
	{
		if (entityExists(e))
		{
			cout << "Entity with id " << e << " therefore we added the component";
			modelComponents[e] = modelComp;
		}
		else
		{
			printf("[WARNING] [EntityManager] can not add model Component. Entity by Id %d does not exist!", e);
		}
	}

	void addMeshComponent( Entity e,  MeshComponent meshComp )
	{
		if (entityExists(e))
		{
			meshComponents[e] = meshComp;
		}
		else
		{
			printf("[WARNING] [EntityManager] can not add mesh Component. Entity by Id %d does not exist!", e);
		}

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
	
	TagComponent* getTagComponent(Entity e)
	{
		auto it = tagComponents.find(e);
		if (it == tagComponents.end())
		{
			return nullptr;
		}
		return &it->second;
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