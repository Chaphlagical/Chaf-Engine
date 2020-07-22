#pragma once

#include <Engine/core.h>
#include <Engine/Layer.h>
#include <Scene/primitive.h>
#include <Scene/model.h>
#include <Renderer/camera.h>
#include <vector>

//TODO: change SceneLayer to SceneLayer

namespace Chaf
{
	class CHAF_API SceneLayer: public Layer
	{
	public:
		SceneLayer();
		~SceneLayer() { m_MeshStack.clear(); }

		virtual void OnAttach() override;
		virtual void OnDetach() override {};

		virtual void OnUpdate(Timestep timestep) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		static SceneLayer* GetInstance() { return s_Instance; }

		std::vector<std::string>& GetMeshNames() { return m_MeshNames; }
		void PushMesh(Ref<TriMesh> mesh) { m_MeshStack.push_back(mesh); m_MeshNames.push_back(mesh->GetName()); }
		Ref<TriMesh> GetSelectMesh() { return m_Select; }
		void SetSelectMesh(uint32_t index) { m_Select = m_MeshStack[index]; }
	private:
		void RenderMesh(Camera& camera);
		void ClearMesh() { m_MeshStack.clear(); }
		void DrawDefaultGrid(Camera& camera) { if(m_ShowGrid) m_Grid->Draw(camera); }
	private:
		std::vector<Ref<TriMesh>> m_MeshStack;
		std::vector<std::string> m_MeshNames;
		bool m_ShowGrid = true;
		Ref<TriMesh> m_Grid;
		static SceneLayer* s_Instance;
		Ref<TriMesh> m_Select;
		Ref<FrameBuffer> m_FrameBuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	};
}