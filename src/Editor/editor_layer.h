#pragma once

#include <Engine/layer.h>
#include <Scene/entity.h>
#include <Scene/scene.h>
#include <Scene/mesh.h>
#include <Renderer/texture.h>
#include <unordered_map>

namespace Chaf
{
	class EditorLayer: public Layer
	{
	public:
		EditorLayer() {}
		~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override {};

		virtual void OnUpdate(Timestep timestep) override {};
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override {};
	private:
		void ShowHierarchy();
		void ShowInspector();
		void ShowHierarchyTree(Entity& node);
		void ShowMenu();
	
	private:
		void ShowTransformComponent();
		void ShowMaterialComponent();
		void ShowMeshComponent();
		void RemoveEntity(Entity& node);
		void AddObject();
		void AddModel();
		void AddComponentMapping(std::string key);

	private:
		Entity m_SelectEntity;
		std::string m_WindowHandle;
		bool m_FlagShowHierarchy = true;
		bool m_FlagShowInspector = true;
		bool m_FlagDemoWindow = false;
		bool m_FlagStyleEditor = false;
	};

}