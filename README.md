# ecs-playground
Weird OOP impmenetation of ECS (Entity-Component-System) in C++ 

![250 sprites](https://github.com/ugozapad/ecs-playground/raw/main/resources/image1.png)

## Design

* Highly inspired by ECS design in [Overwatch](https://www.youtube.com/watch?v=W3aieHjyNvw)

The ECS is made up of the following classes:
- **EntityAdmin** - container of entities, system instances and ComponentPool. 
- **ComponentPool** - container of entity components. 
- **Entity** - Wrapper of entity. Store the id of entity and instance of EntityAdmin.
- **EntityIterator** - Wrapper of entity map iterator.

ECS provide several interfaces for **Components** and **Systems**:
- **IComponentBase** - interface for component.
- **ISystemBase** - interface for system with virtual update method.

Example of usage:
```cpp
// Create the system system
m_pRenderSystem = m_EntityAdmin.CreateSystem< RenderSystem >();

// Getting system
RenderSystem* pRenderSystem = m_EntityAdmin.GetSystem< RenderSystem >();
	
// Create entity
Entity* pEntity = m_EntityAdmin.CreateEntity();

// Create component
TransformComponent* pTransform = pEntity->CreateComponent<TransformComponent>();

// Get component
TransformComponent* pTransform = pEntity->GetComponent<TransformComponent>();

// Iterate though EntityAdmin entities
EntityIterator iterator(m_pEntityAdmin);
for (auto it : iterator)
{
	Entity* pEntity = it.second;
}
```

Example of component:
```cpp
class TransformComponent : public IComponentBase
{
public:
	TransformComponent()
	{
		m_Position = Vec2(0.0f, 0.0f);
		m_Rotation = 0.0f;
	}
	
	~TransformComponent()
	{
	}

public:
    Vec2 m_Position;
    float m_Rotation;
};
```

Example of system:
```cpp
class RotateSystem : public ISystemBase
{
public:
	RotateSystem()
	{
	}
	
	~RotateSystem()
	{
	}

	void Update(float dt)
	{
		EntityIterator iterator(m_pEntityAdmin);
		for (auto it : iterator)
		{
			Entity* pEntity = it.second;
			TransformComponent* pTransform = pEntity->GetComponent<TransformComponent>();
	
			if (pTransform)
				pTransform->m_Rotation += ROTATE_SPEED * dt;
		}
	}
};
```

## How to build

Generate Visual Studio project using these batch files :
* generate_vs2019.bat - Visual Studio 2019
* generate_vs2022.bat - Visual Studio 2022

For anything else you can use premake5 command line arguments.

## Externals

* [Sokol](https://github.com/floooh/sokol) headers
* [premake5](https://premake.github.io/)