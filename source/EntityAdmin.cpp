// EntityAdmin.cpp: implementation of the EntityAdmin class.
//
//////////////////////////////////////////////////////////////////////

#include <algorithm>

#include "EntityAdmin.h"

//////////////////////////////////////////////////////////////////////
// Entity
//////////////////////////////////////////////////////////////////////

Entity::Entity()
{
    m_EntityId = -1;
    m_pEntityAdmin = NULL;
}

Entity::~Entity()
{
}

Entity& Entity::operator=(const Entity& rhs)
{
    m_EntityId = rhs.m_EntityId;
    m_pEntityAdmin = rhs.m_pEntityAdmin;
    return *this;
}

void Entity::RemoveComponent(IComponentBase* pComponent)
{
    if (!m_pEntityAdmin)
        return;

    m_pEntityAdmin->RemoveComponent(this, pComponent);
}

EntityAdmin* Entity::GetEntityAdmin()
{
    return m_pEntityAdmin;
}

EntityId_t Entity::GetEntityId()
{
    return m_EntityId;
}

//////////////////////////////////////////////////////////////////////
// EntityAdmin
//////////////////////////////////////////////////////////////////////

EntityAdmin::EntityAdmin()
{
}

EntityAdmin::~EntityAdmin()
{
	for ( int i = 0; i < m_Systems.size(); i++ )
	{
		if ( m_Systems[ i ] )
			delete m_Systems[ i ];
	}
	
	m_Systems.clear();
}

Entity* EntityAdmin::CreateEntity()
{
	static int s_EntityId = 0;

	Entity* pEntity = new Entity();
	pEntity->m_pEntityAdmin = this;
	pEntity->m_EntityId = s_EntityId;
	m_Entities[ s_EntityId ] = pEntity;

	s_EntityId++;

	return pEntity;
}

void EntityAdmin::RemoveEntity(Entity* pEntity)
{
    // remove all entity components
    m_ComponentPool.RemoveComponents(pEntity->GetEntityId());

    // erase from map
    m_Entities.erase(pEntity->GetEntityId());

    // delete wrapper
    delete pEntity;
}

void EntityAdmin::RemoveEntities()
{
    size_t numEntities = m_Entities.size();

    std::vector<Entity*> entities;
    entities.resize(numEntities);

    int cnt = 0;
    for (const auto& entity : m_Entities)
    {
        entities[cnt] = entity.second;
        cnt++;
    }

    for (int i = 0; i < entities.size(); i++)
	{
	    RemoveEntity(entities[i]);
	}

	m_Entities.clear();
}

void EntityAdmin::AddSystem( ISystemBase* pSystem )
{
	m_Systems.push_back( pSystem );
}

void EntityAdmin::Update( float dt )
{
	for ( int i = 0; i < m_Systems.size(); i++ )
	{
		if ( m_Systems[ i ] )
			 m_Systems[ i ]->Update( dt );
	}
}

void EntityAdmin::RemoveComponent(Entity* pEntity, IComponentBase* pComponent)
{
    m_ComponentPool.RemoveComponent(pEntity->GetEntityId(), pComponent);
}

//////////////////////////////////////////////////////////////////////
// Component Pool
//////////////////////////////////////////////////////////////////////

ComponentPool::ComponentPool()
{
}

ComponentPool::~ComponentPool()
{

}

void ComponentPool::RemoveComponent(EntityId_t entity, IComponentBase* pComponent)
{
    std::map<EntityId_t, Components>::iterator it = m_ComponentMap.find(entity);
    if (it == m_ComponentMap.end())
        return;

    Components& components = (*it).second;
    Components::iterator component = std::find(components.begin(), components.end(), pComponent);
    if (component == components.end())
        return;

    delete pComponent;

    components.erase(component);
}

void ComponentPool::RemoveComponents(EntityId_t entity)
{
    std::map<EntityId_t, Components>::iterator it = m_ComponentMap.find(entity);
    if (it == m_ComponentMap.end())
        return;

    Components& components = (*it).second;
    for (int i = 0; i < components.size(); i++)
    {
        if (components[i])
            delete components[i];
    }

    components.clear();
}

bool ComponentPool::HasComponent(EntityId_t entity, IComponentBase* pComponent)
{
    std::map<EntityId_t, Components>::iterator it = m_ComponentMap.find(entity);
    if (it == m_ComponentMap.end())
        return false;

    const Components& components = (*it).second;
    for (int i = 0; i < components.size(); i++)
    {
        if (components[i] == pComponent)
            return true;
    }

    return false;
}
