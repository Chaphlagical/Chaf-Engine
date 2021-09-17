#pragma once

#include <Engine/core.h>
#include <Scene/entity.h>
#include <Scene/components.h>
#include <Renderer/mesh.h>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include <imgui.h>
#include <Gui/Guizmo/ImGuizmo.h>

namespace Chaf
{
	static std::unordered_map<MeshType, std::string> MeshType2Item =
	{
		{MeshType::None, "Empty"},
		{MeshType::Plane, "Plane"},
		{MeshType::Cube, "Cube"},
		{MeshType::Sphere, "Sphere"},
		{MeshType::Model, "Model"},
	};
	static std::unordered_map<std::string, MeshType> MeshItem2Type =
	{
		{"Empty", MeshType::None},
		{"Plane", MeshType::Plane},
		{"Cube", MeshType::Cube},
		{"Sphere", MeshType::Sphere},
		{ "Model", MeshType::Model}
	};

	/*
		m_PopupFlag: for
	*/

	class CHAF_API EditorBasic
	{
	public:
		static void SetSelectEntity(Entity& entity) { m_SelectEntity = entity; }
		static void SetPopupFlag(const std::string& flag) { m_PopupFlag = flag; }
		static Entity& GetSelectEntity() { return m_SelectEntity; }

		static void GetFileDialog(const std::string& label, const std::string& format, std::function<void(const std::string&)> func);

		static void ShowTexture(const char* label, const Ref<Texture2D>& texture, std::function<void(void)> func = []() {});

		static void Rename(Entity& entity);

		static void AddObjectMenu(Entity& entity);
		static void AddObjectAnswer();

		template<typename T>
		static void SetDragDropSource(const char* label, std::function<void(void)> func, T& data)
		{
			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload(label, &data, sizeof(T));
				func();
				ImGui::EndDragDropSource();
			}
		}

		template<typename T>
		static void SetDragDropTarget(const char* label, std::function<void(T&)> func)
		{
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(label))
				{
					CHAF_CORE_ASSERT(payload->DataSize == sizeof(T), "payload error!");
					T data = *(const T*)payload->Data;
					func(data);
				}
				ImGui::EndDragDropTarget();
			}
		}
		
		template<typename T>
		static void AddComponent()
		{
			if (!EditorBasic::GetSelectEntity().HasComponent<T>() && !EditorBasic::GetSelectEntity().IsRoot())
				EditorBasic::GetSelectEntity().AddComponent<T>();
		}

	private:
		static Entity m_SelectEntity;
		static std::string m_PopupFlag;
	public:
		static bool m_FlagShowHierarchy;
		static bool m_FlagShowInspector;
		static bool	m_FlagDemoWindow;
		static bool m_FlagStyleEditor;
		static bool m_FlagTerminal;

		static ImGuizmo::OPERATION mCurrentGizmoOperation;
		static ImGuizmo::MODE mCurrentGizmoMode;

		static bool m_InitFileDialog;
	};

}