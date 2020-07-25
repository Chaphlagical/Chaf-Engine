#include <Scene/scene.h>
#include <Scene/entity.h>
#include <Scene/components.h>
#include <Renderer/command.h>

namespace Chaf
{
	Scene::Scene()
	{
		entt::entity entityIndex = m_Registry.create();
		Entity entity = { entityIndex, this };
		entity.AddComponent<Relationship>();
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = "scene";
		m_Root = entityIndex;

		m_DefaultTexture = Texture2D::Create(1, 1);
		uint32_t defaultData = 0xffffffff;
		m_DefaultTexture->SetData(&defaultData, sizeof(uint32_t));
		m_DefaultColor = glm::vec4{ 1.0f };
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name, entt::entity parent)
	{
		if(parent==entt::null)parent=m_Root;
		entt::entity entityIndex = m_Registry.create();
		Entity entity = { entityIndex, this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<Relationship>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		auto root = Entity(parent, this);
		root.AddChild(entity);
		return entity;
	}

	void Scene::OnUpdate(Timestep ts, Ref<Shader>& shader)
	{

		auto group=m_Registry.group<TagComponent, TransformComponent>(entt::get<>);
		for (auto entity : group)
		{
			auto& transform = group.get<TransformComponent>(entity);
			transform.Update();
			shader->SetMat4("u_Transform", transform);

			if (m_Registry.has<MaterialComponent>(entity))
			{
				auto& material = m_Registry.get<MaterialComponent>(entity);
				material.Bind(shader);
				shader->SetFloat4("u_Color", material.Color);
			}
			else
			{
				m_DefaultTexture->Bind(0);
				shader->SetFloat4("u_Color", glm::vec4(1.0f));
			}

			if (m_Registry.has<MeshComponent>(entity))
			{
				auto& mesh = m_Registry.get<MeshComponent>(entity);
				mesh.Mesh->Draw();
			}
		}
	}

	Entity Scene::GetRoot()
	{
		//CHAF_CORE_ASSERT(m_Root == entt::null, "root doesn't exist!");
		return { m_Root,this };
	}
}