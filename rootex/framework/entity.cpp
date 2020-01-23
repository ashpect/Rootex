#include "entity.h"

#include "framework/component.h"
#include "framework/system.h"

void Entity::addComponent(const Ref<Component>& component)
{
	m_Components.insert(std::make_pair(component->getComponentID(), component));
}

Entity::Entity(EntityID id)
    : m_ID(id)
{
}

bool Entity::setupComponents()
{
	bool status = true;
	for (auto& component : m_Components)
	{
		status = status & component.second->setup();
	}
	return status;
}

void Entity::destroy()
{
	for (auto& component : m_Components)
	{
		System::DeregisterComponent(component.second.get());
		component.second.reset();
	}
	m_Components.clear();
}

EntityID Entity::getID() const
{
	return m_ID;
}

const HashMap<ComponentID, Ref<Component>>& Entity::getAllComponents() const
{
	return m_Components;
}