#pragma once
#include <glm/glm.hpp>

#include "Hazel/Scene/SceneCamera.h"
#include "Hazel/Scene/ScriptableEntity.h"

namespace Hazel
{
	struct NameComponent
	{
		std::string name = "";
		NameComponent() = default;
		NameComponent(const NameComponent& other) :
			name(other.name) {};
		NameComponent(const std::string& name) :
			name(name) {}

		operator const std::string& () const { return name; }
		operator std::string& () { return name; }
	};

	struct TransformComponent
	{
		glm::mat4 transform = glm::mat4(1.f);
		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) :
			transform(other.transform) {};
		TransformComponent(const glm::mat4& trans) :
			transform(trans) {}

		operator const glm::mat4& () const { return transform; }
		operator glm::mat4& () { return transform; }
	};

	struct SpriteRenderComponent
	{
		glm::vec4 color = {1.f, 1.f, 1.f, 1.f};
		SpriteRenderComponent() = default;
		SpriteRenderComponent(const SpriteRenderComponent& other) :
			color(other.color) {};
		SpriteRenderComponent(const glm::vec4& color) :
			color(color) {}

		operator const glm::vec4& () const { return color; }
		operator glm::vec4& () { return color; }
	};

	struct CameraComponent
	{
		SceneCamera camera;

		// Scene 내 여러 개 CameraComponent  가 있을 수 있다.
		// MainCamera 를 뭐라고 세팅할 것인가.
		bool isPrimary = true;

		// true 라면, viewport resize 에 영향 받지 않게 하고
		// false 라면, viewport resize 에 영향을 받게 한다.
		bool isFixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent& other) :
			camera(other.camera) {};
		CameraComponent(const glm::mat4& projection) :
			camera(projection) {}
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* m_Instance = nullptr;

		// std::function 대신에 함수 포인터를 직접 사용한다. (깔끔하고 가볍다)
		// std::function<void()> OnInstantiateScript;
		ScriptableEntity* (*OnInstantiateScript)();

		void(*OnDestroyScript)(NativeScriptComponent*);

		// std::function<void(ScriptableEntity*)> OnCreateFunction;
		// std::function<void(ScriptableEntity*)> OnDestroyFunction;
		// std::function<void(ScriptableEntity*, Timestep)> OnUpdateFunction;

		template<typename T>
		void Bind()
		{
			OnInstantiateScript = []() {return static_cast<ScriptableEntity*>(new T()); };
			OnDestroyScript = [](NativeScriptComponent* comp) {delete comp->m_Instance; comp->m_Instance = nullptr; };

			// instance 의 type 은 T type 일 것이다.
			// OnCreateFunction = [&](ScriptableEntity* instance) {((T*)instance)->OnCreate(); };
			// OnDestroyFunction = [&](ScriptableEntity* instance) {((T*)instance)->OnDestroy(); };
			// OnUpdateFunction = [&](ScriptableEntity* instance, Timestep ts) {((T*)instance)->OnUpdate(ts); };
		}
	};
}