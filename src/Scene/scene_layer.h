#pragma once

#include <Engine/core.h>
#include <Engine/Layer.h>
#include <Scene/primitive.h>
#include <Scene/model.h>
#include <Renderer/camera.h>
#include <vector>
#include <unordered_map>

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

		std::unordered_map<std::string, uint32_t>& GetMeshNames() { return m_MeshNames; }
		void PushMesh(Ref<TriMesh> mesh) 
		{ 
			m_MeshStack.push_back(mesh); 
			if (m_MeshNames.count(mesh->GetName()) != 0)
			{
				int count = 1;
				while (m_MeshNames.count(mesh->GetName() + "(" + std::to_string(count) + ")") != 0)
					count++;
				mesh->SetName(mesh->GetName() + "(" + std::to_string(count) + ")");
			}
			m_MeshNames[mesh->GetName()]=m_MeshNames.size(); 
			m_Select = mesh->GetName();
		}
		void PopMesh(std::string name)
		{
			if (m_MeshNames.count(name) == 0)return;
			uint32_t index = m_MeshNames[name];
			m_MeshNames.erase(name);
			for (auto& name : m_MeshNames)
				if (name.second > index)name.second--;
			m_MeshStack.erase(m_MeshStack.begin() + index);
			if (name == m_Select)m_Select = "";
		}
		Ref<TriMesh>& GetSelectMesh() { if (m_MeshNames.count(m_Select) == 0) CHAF_CORE_ASSERT(false, "mesh not exists!"); return m_MeshStack[m_MeshNames[m_Select]]; }
		bool& GetLineMode() { return m_LineMode; }
		const uint32_t GetMeshNumber() const { return m_MeshStack.size(); }
		bool IsSelectValid() { return (m_MeshNames.count(m_Select)); }
		bool& IsShowGrid() { return m_ShowGrid; }
		void SetShowGrid(const bool enable) { m_ShowGrid = enable; }
		void SetLineMode(const bool enable) { m_LineMode = enable; }
		void SetSelectMesh(std::string name) { m_Select = name; }
		void RenameMesh(std::string src, std::string dst)	//	src -> dst
		{
			if (src == dst)return;
			if (m_MeshNames.count(src) == 0)
			{
				CHAF_ERROR("no " + src + " in mesh stack!");
				return;
			}
			uint32_t index = m_MeshNames[src];
			if (m_MeshNames.count(dst) != 0)
			{
				int count = 1;
				while (m_MeshNames.count(dst + "(" + std::to_string(count) + ")") != 0)
					count++;
				dst = dst + "(" + std::to_string(count) + ")";
				m_MeshStack[index]->SetName(dst);
			}
			auto iter=m_MeshNames.erase(src);
			m_MeshNames[dst] = index;
			if (m_Select == src)m_Select = dst;
		}
		Ref<Texture2D> GetDefaultDisplayTexture() { return m_DefaultDisplayTexture; }
	private:
		void RenderMesh(Camera& camera);
		void ClearMesh() { m_MeshStack.clear(); }
		void DrawDefaultGrid(Camera& camera) { if(m_ShowGrid) m_Grid->Draw(camera); }
	private:
		std::vector<Ref<TriMesh>> m_MeshStack;
		std::unordered_map<std::string, uint32_t> m_MeshNames;
		Ref<TriMesh> m_Grid;
		Ref<FrameBuffer> m_FrameBuffer;
		bool m_ShowGrid = true;
		bool m_LineMode = false;
		static SceneLayer* s_Instance;
		std::string m_Select = "";
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		Ref<Texture2D> m_DefaultDisplayTexture;
	};
}