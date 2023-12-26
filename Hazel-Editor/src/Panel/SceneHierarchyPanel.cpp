﻿#include "hzpch.h"

// Panel
#include "SceneHierarchyPanel.h"
#include "ComponentPanel/CameraPanel.h"
#include "Utils/PanelUtils.h"

// Component
#include "Hazel/Scene/Component/Component.h"
#include "Hazel/Scene/Component/Renderer/SpriteRenderComponent.h"
#include "Hazel/Scene/Component/Identifier/NameComponent.h"
#include "Hazel/Scene/Component/TransformComponent.h"
#include "Hazel/Scene/Component/CameraComponent.h"
#include "Hazel/Scene/Component/NativeScriptComponent.h"
#include "Hazel/Scene/Component/RigidBody2DComponent.h"
#include "Hazel/Scene/Component/Collider/BoxCollider2DComponent.h"
#include "Hazel/Scene/Component/Collider/CircleCollider2DComponent.h"
#include "Hazel/Scene/Component/Renderer/CircleRendererComponent.h"
#include <filesystem>
#include <imgui/imgui.h>

namespace HazelEditor {
	extern const std::filesystem::path g_AssetPath;

}
namespace Hazel
{
	using HazelEditor::g_AssetPath;

	void SceneHierarchyPanel::SetContext(const  std::weak_ptr<Scene>& scene)
	{
		m_Context = scene;

		m_SelectedEntity = {};
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectedEntity = entity;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		Ref<Scene> sceneContext = m_Context.lock();

		if (!sceneContext)
		{
			assert(false);
		}

		{
			// Scene Hierarchy
			ImGui::Begin("SceneHierarchy");

			// Scene Name
			{
				// 여기서 name 을 수정할 수 있게 한다.
				const std::string& name = sceneContext->GetName();

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), name.c_str());

				ImGui::Text("SceneName");

				ImGui::SameLine();

				if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
				{
					sceneContext->SetName(std::string(buffer));
				}
			}
		

			sceneContext->m_Registry.each([&](auto entityID)
				{
					Entity entity{ entityID, m_Context.lock().get()};
					drawEntityNode(entity);
				});

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_SelectedEntity = {};

			// Right Click on Black Space
			if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
			{
				if (ImGui::MenuItem("Create Empty Entity"))
					sceneContext->CreateEntity();
		
				ImGui::EndPopup();
			}

			ImGui::End();
		}


		{
			// Property
			ImGui::Begin("Properties");

			if (m_SelectedEntity)
			{
				drawComponents(m_SelectedEntity);
			}

			ImGui::End();
		}

	}

	// draw selectable list of entities
	void SceneHierarchyPanel::drawEntityNode(Entity entity)
	{
		Ref<Scene> sceneContext = m_Context.lock();

		if (!sceneContext)
		{
			assert(false);
		}

		std::string& name = const_cast<std::string&>(
			entity.GetComponent<NameComponent>().GetName());

		// ImGuiTreeNodeFlags_OpenOnArrow : arrow 를 클릭해야만 해당 내용 모두 펼쳐서 볼 수 있다.
		//	ImGuiTreeNodeFlags_Selected			: selected 된 채로 보이게 된다.
		ImGuiTreeNodeFlags flags = 
			(m_SelectedEntity == entity ?  ImGuiTreeNodeFlags_Selected : 0) |
			ImGuiTreeNodeFlags_OpenOnArrow;

		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		
		bool isOpened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());
		bool entityDeleted = false;

		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}
		
		if (isOpened)
		{
			{
				// Tree 안에 Tree Node 가 보일 수 있게 한다.
				ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
				bool isOpened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());
				flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
				if (isOpened)
				{

					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			sceneContext->DestroyEntity(entity);

			// m_SelectedEntity ? == 현재 클릭된 Entity 의 Component 목록을 보고 있었다는 의미
			// m_SelectedEntity 정보를 비워준다.
			if (m_SelectedEntity == entity)
				m_SelectedEntity = {};
		}
	}

	void SceneHierarchyPanel::drawComponents(Entity entity)
	{
		drawNameComponent(entity);

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		drawAddComponentPanel();

		ImGui::PopItemWidth();

		drawTransformComponent(entity);


		drawRendererComponent(entity);
		drawRigidBodyComponent(entity);
		drawColliderComponent(entity);

	}

	void SceneHierarchyPanel::drawNameComponent(Entity entity)
	{
		if (entity.HasComponent<NameComponent>())
		{
			// 여기서 name 을 수정할 수 있게 한다.
			std::string& name = const_cast<std::string&>(
				entity.GetComponent<NameComponent>().GetName());

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), name.c_str());

			if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
			{
				name = std::string(buffer);
			}
		}
	}

	void SceneHierarchyPanel::drawTransformComponent(Entity entity)
	{
		HazelEditor::DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {
			HazelEditor::DrawVec3Control("Translation", component.GetTranslationRef());
			glm::vec3 degree = glm::degrees(component.GetRotationRef());
			HazelEditor::DrawVec3Control("Rotation", degree);
			// component.Rotation = glm::radians(degree);
			component.SetRotation(glm::radians(degree));
			HazelEditor::DrawVec3Control("Scale", component.GetScaleRef());
			});
	}

	void SceneHierarchyPanel::drawCameraComponent(Entity entity)
	{
		HazelEditor::DrawComponent<CameraComponent>("Camera", entity, &HazelEditor::CameraPanel::DrawCameraComponent);
	}

	void SceneHierarchyPanel::drawRendererComponent(Entity entity)
	{

		HazelEditor::DrawComponent<CircleRendererComponent>("Circle Renderer", entity, [](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.GetColorRef()));
				ImGui::DragFloat("Thickness", &component.GetThickNessRef(), 0.025f, 0.0f, 1.0f);
				ImGui::DragFloat("Fade", &component.GetFadeRef(), 0.00025f, 0.0f, 1.0f);
			});
		/*
		DrawComponent<CameraComponent>("Camera", entity, [](auto& component) {
			SceneCamera& camera = const_cast<SceneCamera&>(
				component.GetCamera());

			ImGui::Checkbox("Primary", &component.GetPrimaryRef());

			const char* projectionTypeStrings[] = { "Projection", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

			if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
			{
				for (int i = 0; i < 2; ++i)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];

					if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
					{
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			};

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Projective)
			{
				float perspectiveFov = glm::degrees(camera.GetPerspectiveFov());
				float perspectiveNear = camera.GetPerspectiveNear();
				float perspectiveFar = camera.GetPerspectiveFar();

				if (ImGui::DragFloat("Fov", &perspectiveFov))
				{
					camera.SetPerspectiveFov(glm::radians(perspectiveFov));
				}

				if (ImGui::DragFloat("Near", &perspectiveNear))
				{
					camera.SetPerspectiveNearClip(perspectiveNear);
				}

				if (ImGui::DragFloat("Far", &perspectiveFar))
				{
					camera.SetPerspectiveFarClip(perspectiveFar);
				}
			}
			else if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float orthoSize = camera.GetOrthoGraphicSize();
				float orthoNear = camera.GetOrthographicNear();
				float orthoFar = camera.GetOrthographicFar();

				if (ImGui::DragFloat("Size", &orthoSize))
				{
					camera.SetOrthoGraphicSize(orthoSize);
				}

				if (ImGui::DragFloat("Near", &orthoNear))
				{
					camera.SetOrthographicNearClip(orthoNear);
				}

				if (ImGui::DragFloat("Far", &orthoFar))
				{
					camera.SetOrthographicFarClip(orthoFar);
				}
			}

			ImGui::Checkbox("Fixed Aspect Ratio", &component.GetFixedAspectRatioRef());
		});
		*/

		HazelEditor::DrawComponent<SpriteRenderComponent>("Sprite", entity, [](auto& component) {
			ImGui::ColorEdit4("Color", glm::value_ptr(component.GetColorRef()));

			ImGui::Button("Texture", ImVec2(100.0f, 0.0f));

			// Drag In Texture for entity
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
					// component.Texture = Texture2D::Create(texturePath.string());
					Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
					if (texture->IsLoaded())
					{
						component.SetTexture(texture);
					}
					else
					{
						HZ_WARN("Could not load texture {0}", texturePath.filename().string());
					}
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::DragFloat("Tiling Factor", &component.GetTilingFactorRef(), 0.1f, 0.0f, 100.0f);
			});
	}

	void SceneHierarchyPanel::drawColliderComponent(Entity entity)
	{

		HazelEditor::DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.GetOffsetRef()));
				ImGui::DragFloat2("Size", glm::value_ptr(component.GetSizeRef()));
				ImGui::DragFloat("Density", &component.GetDensityRef(), 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.GetFrictionRef(), 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.GetRestitutionRef(), 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution Threshold", &component.GetRestitutionThresholdRef(), 0.01f, 0.0f);
			});

		HazelEditor::DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [](auto& component)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.GetOffsetRef()));
				ImGui::DragFloat("Radius", &component.GetRadiusRef());
				ImGui::DragFloat("Density", &component.GetDensityRef(), 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.GetFrictionRef(), 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.GetRestitutionRef(), 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution Threshold", &component.GetRestitutionThresholdRef(), 0.01f, 0.0f);
			});
	}

	void SceneHierarchyPanel::drawRigidBodyComponent(Entity entity)
	{
		HazelEditor::DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity, [](auto& component)
			{
				const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
				const char* currentBodyTypeString = bodyTypeStrings[(int)component.GetBodyType()];
				if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
						if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
						{
							currentBodyTypeString = bodyTypeStrings[i];
							// component.Type = (Rigidbody2DComponent::BodyType)i;
							component.SetBodyType((Rigidbody2DComponent::BodyType)i);
						}

						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				ImGui::Checkbox("Fixed Rotation", &component.GetFixedRotationRef());
			});
	}

	void SceneHierarchyPanel::drawAddComponentPanel()
	{
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (!m_SelectedEntity.HasComponent<CircleRendererComponent>())
			{
				if (ImGui::MenuItem("Circle Renderer"))
				{
					m_SelectedEntity.AddComponent<CircleRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
			}
			if (!m_SelectedEntity.HasComponent<CameraComponent>())
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_SelectedEntity.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_SelectedEntity.HasComponent<SpriteRenderComponent>())
			{
				if (ImGui::MenuItem("SpriteRenderer"))
				{
					m_SelectedEntity.AddComponent<SpriteRenderComponent>();
					ImGui::CloseCurrentPopup();
				}
			}
			if (!m_SelectedEntity.HasComponent<Rigidbody2DComponent>())
			{
				if (ImGui::MenuItem("Rigidbody 2D"))
				{
					m_SelectedEntity.AddComponent<Rigidbody2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_SelectedEntity.HasComponent<BoxCollider2DComponent>())
			{
				if (ImGui::MenuItem("Box Collider 2D"))
				{
					m_SelectedEntity.AddComponent<BoxCollider2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}
			if (!m_SelectedEntity.HasComponent<CircleCollider2DComponent>())
			{
				if (ImGui::MenuItem("Circle Collider 2D"))
				{
					m_SelectedEntity.AddComponent<CircleCollider2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}
	}

}


