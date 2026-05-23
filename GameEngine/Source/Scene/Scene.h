#pragma once

#include "EntityManager.h"
#include "Renderer/Renderer.h"
#include "Core/Systems/RenderSystem.h"
#include "Core/Systems/TransformSystem.h"
class Scene
{
public:
	
	//Calls update on the core systems
	void update( Renderer &renderer )
	{
		transformSystem.update( entityManager );
		renderSystem.update( entityManager, renderer);
	}
	
	Entity createEntity()
	{
		return entityManager.createEntity();
	}
	//Adds a tag component to an entity in the scene
	void addTag(Entity e, std::string tag)
	{
		entityManager.addTagComponent(e, TagComponent{ tag });
	}
	//Adds a transform component to an entity in the scene
	void addTransform( Entity e, const TransformComponent& transform )
	{
		entityManager.addTransformComponent( e, transform );
	}
	//Adds a model component to an entity in the scene
	void addModel( Entity e, const ModelComponent& modelComp )
	{
		entityManager.addModelComponent( e, modelComp );
	}
	//Adds a mesh component to an entity in the scene
	void addMesh( Entity e, const MeshComponent& meshComp )
	{
		entityManager.addMeshComponent(e, meshComp);
	}

	TransformComponent* getTransformComp( Entity e )
	{
		return entityManager.getTransformComponent( e );
	}

	ModelComponent* getModelComp( Entity e )
	{
		return entityManager.getModelComponent(e);
	}

	MeshComponent* getMeshComp( Entity e)
	{
		return entityManager.getMeshComponent(e);
	}

	EntityManager& getEntityManager()
	{
		return entityManager;
	}

private:
	EntityManager entityManager;
	RenderSystem renderSystem;
	TransformSystem transformSystem;
};