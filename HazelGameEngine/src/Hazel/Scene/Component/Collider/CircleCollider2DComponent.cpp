#include "hzpch.h"
#include "CircleCollider2DComponent.h"
#include "Hazel/Core/Serialization/Serializer.h"


Hazel::CircleCollider2DComponent::CircleCollider2DComponent()
{
    Reflection::RegistType<CircleCollider2DComponent>();

}

Hazel::CircleCollider2DComponent::CircleCollider2DComponent(const CircleCollider2DComponent&)
{
    Reflection::RegistType<CircleCollider2DComponent>();
}

void Hazel::CircleCollider2DComponent::Serialize(Serializer* serializer)
{
	serializer->BeginSaveMap(Reflection::GetTypeID<CircleCollider2DComponent>(), this);
	Reflection::TypeInfo* compTypeInfo = Reflection::GetTypeInfo(GetType());
	serializer->Save("compName", compTypeInfo->m_Name.c_str());

	serializer->Save("Offset", m_Offset);
	serializer->Save("Radius", m_Radius);

	serializer->Save("Density", m_Density);
	serializer->Save("Friction", m_Friction);
	serializer->Save("Restitution", m_Restitution);
	serializer->Save("RestitutionThreshold", m_RestitutionThreshold);

	serializer->EndSaveMap();
}

void Hazel::CircleCollider2DComponent::Deserialize(Serializer* serializer)
{
	serializer->BeginLoadMap(Reflection::GetTypeID<CircleCollider2DComponent>(), this);
	std::string compName;
	serializer->Load("compName", compName);

	serializer->Load("Offset", m_Offset);
	serializer->Load("Radius", m_Radius);

	serializer->Load("Density", m_Density);
	serializer->Load("Friction", m_Friction);
	serializer->Load("Restitution", m_Restitution);
	serializer->Load("RestitutionThreshold", m_RestitutionThreshold);


	serializer->EndLoadMap();
}

const TypeId Hazel::CircleCollider2DComponent::GetType() const
{
	return Reflection::GetTypeID<CircleCollider2DComponent>();
}
