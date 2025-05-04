// EntityAdmin.h: interface for the EntityAdmin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(ENTITYADMIN_H__INCLUDED_)
#define ENTITYADMIN_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <vector>

typedef unsigned int EntityId_t;

class EntityAdmin;
class IComponentBase;

class ComponentPool
{
public:
    typedef std::vector<IComponentBase*> Components;

public:
    ComponentPool();
    ~ComponentPool();

    template <typename T>
    IComponentBase* CreateComponent(EntityId_t entity);

    void RemoveComponent(EntityId_t entity, IComponentBase* pComponent);
    void RemoveComponents(EntityId_t entity);

	template <typename T>
	T* GetComponent(EntityId_t entity);
	
	bool HasComponent(EntityId_t entity, IComponentBase* pComponent);

private:
    std::map<EntityId_t, Components> m_ComponentMap;
};

template <typename T>
IComponentBase* ComponentPool::CreateComponent(EntityId_t entity)
{
    // try to find pool
    std::map<EntityId_t, Components>::iterator it = m_ComponentMap.find(entity);
    if (it == m_ComponentMap.end())
    {
        // create new pool for entity
        Components components;
        m_ComponentMap.emplace(entity, components);
    }

    // access to entity component pool
    Components& components = m_ComponentMap[entity];

    // create component
    IComponentBase* pComponent = new T();

    components.push_back(pComponent);

    return pComponent;
}

template<typename T>
inline T* ComponentPool::GetComponent(EntityId_t entity)
{
	// try to find pool
	std::map<EntityId_t, Components>::iterator it = m_ComponentMap.find(entity);
	if (it == m_ComponentMap.end())
		return nullptr;

	// access to entity component pool
	Components& components = m_ComponentMap[entity];

	for (int i = 0, size = components.size(); i < size; i++)
	{
		T* pComponent = dynamic_cast<T*>(components[i]);
		if (pComponent)
			return pComponent;
	}

	return nullptr;
}

class Entity
{
	friend class EntityAdmin;
public:
	Entity();
	~Entity();

	Entity& operator=(const Entity& rhs);

	template <typename T>
	T* CreateComponent();

	// Return the first component of the template type.
	template <typename T>
	T* GetComponent();

	void RemoveComponent(IComponentBase* pComponent);

	EntityAdmin*	GetEntityAdmin();
	EntityId_t		GetEntityId();

private:
	EntityAdmin*	m_pEntityAdmin;
	EntityId_t		m_EntityId;
};

template <typename T>
T* Entity::CreateComponent()
{
	if (!m_pEntityAdmin)
		return NULL;
	
	return m_pEntityAdmin->CreateComponent<T>(this);
}

template<typename T>
inline T* Entity::GetComponent()
{
	if (!m_pEntityAdmin)
		return NULL;

	return m_pEntityAdmin->GetComponent<T>(this);
}

class IComponentBase
{
public:
	virtual ~IComponentBase() {}
};

class ISystemBase
{
public:
	virtual ~ISystemBase() {}
	virtual void Update( float dt ) = 0;

protected:
	friend class EntityAdmin;
	EntityAdmin* m_pEntityAdmin = NULL;
};

class EntityAdmin
{
public:
	EntityAdmin();
	~EntityAdmin();

	Entity* CreateEntity();
    void RemoveEntity(Entity* pEntity);
    void RemoveEntities();

	void AddSystem( ISystemBase* pSystem );

	void Update( float dt );

	template <typename T>
	T* CreateComponent(Entity* pEntity);

	void RemoveComponent(Entity* pEntity, IComponentBase* pComponent);

	template <typename T>
	T* CreateSystem();

	template <typename T>
	T* GetSystem();

	// Component accessing

	// Accessing the entities
	const std::map< EntityId_t, Entity* >& GetEntities() const { return m_Entities; }

	// Accessing the Component Pool
	ComponentPool* GetComponentPool() { return &m_ComponentPool; }

    // Return the first component of the template type.
    template <typename T>
	T* GetComponent(Entity* pEntity);

private:
	std::map< EntityId_t, Entity* > m_Entities;
	std::vector< ISystemBase* > m_Systems;
	ComponentPool m_ComponentPool;
};

template <typename T>
T* EntityAdmin::CreateComponent(Entity* pEntity)
{
	if (!pEntity)
		return NULL;
	
	return (T*)GetComponentPool()->CreateComponent<T>(pEntity->GetEntityId());
}

template <typename T>
T* EntityAdmin::CreateSystem()
{
	T* pSystem = new T();
	pSystem->m_pEntityAdmin = this;
	m_Systems.push_back( pSystem );
	return pSystem;
}

template <typename T>
T* EntityAdmin::GetSystem()
{
	for ( int i = 0; i < m_Systems.size(); i++ )
	{
		T* pSystem = dynamic_cast< T* >( m_Systems[ i ] );
		if ( pSystem )
			return pSystem;
	}

	return NULL;
}

template <typename T>
T* EntityAdmin::GetComponent(Entity* pEntity)
{
    return (T*)GetComponentPool()->GetComponent<T>(pEntity->GetEntityId());
}

class EntityIterator
{
public:
	EntityIterator(const EntityAdmin* pEntityAdmin) :
		m_pEntityAdmin(pEntityAdmin)
	{
		m_Iterator = m_pEntityAdmin->GetEntities().begin();
	}

	~EntityIterator()
	{
		m_pEntityAdmin = NULL;
	}

	std::map< EntityId_t, Entity* >::const_iterator  begin() { return m_Iterator; }
	std::map< EntityId_t, Entity* >::const_iterator  end() { return m_pEntityAdmin->GetEntities().end(); }

private:
	typedef std::map< EntityId_t, Entity* >::const_iterator EntityMapIt;

	const EntityAdmin* m_pEntityAdmin;
	EntityMapIt m_Iterator;
};

#endif // !defined(ENTITYADMIN_H__INCLUDED_)
