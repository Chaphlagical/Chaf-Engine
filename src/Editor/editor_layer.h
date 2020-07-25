#pragma once

#include <Engine/layer.h>
#include <Scene/entity.h>
#include <Scene/scene.h>
#include <Scene/mesh.h>
#include <Renderer/texture.h>

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
	public:
		void ShowHierarchy();
		void ShowInspector();
		void ShowHierarchyTree(Entity& node);
	
	private:
		void ShowTransformComponent();
		void ShowMaterialComponent();
		void RemoveEntity(Entity& node);
		void AddEntity(Entity& node);
		void AddObject();
		void AddModel();

	private:
		Entity m_SelectEntity;
		std::string m_WindowHandle;
		
	};

}