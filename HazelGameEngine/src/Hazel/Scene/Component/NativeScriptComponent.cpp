﻿#include "hzpch.h"
#include "NativeScriptComponent.h"
#include "Hazel/Core/Reflection/Reflection.h"
#include "Hazel/Core/Serialization/Serializer.h"

Hazel::NativeScriptComponent::NativeScriptComponent()
{
	Reflection::RegistType<NativeScriptComponent>();
}

const TypeId Hazel::NativeScriptComponent::GetType() const
{
	return Reflection::GetTypeID<NativeScriptComponent>();
}
