#pragma once

#include <entt.hpp>
#include <Engine/core.h>
#include <Engine/time.h>
#include <Renderer/shader.h>
#include <Renderer/texture.h>

namespace Chaf
{
	struct Relationship
	{
		size_t children{};
		entt::entity first{ entt::null };
		entt::entity prev{ entt::null };
		entt::entity next{ entt::null };
		entt::entity parent{ entt::null };
	};

	class Entity;

	class Scene
	{
		friend class Entity;
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string(), entt::entity parent = entt::null);

		void OnUpdate(Timestep ts, Ref<Shader>& shader);

		Entity GetRoot();
		bool Empty() { return (m_Root == entt::null); }
		void SetLineMode(const bool& enable) { m_LineMode = enable; }
		bool GetLineMode() { return m_LineMode; }

	private:
		entt::registry m_Registry;
		entt::entity m_Root = entt::null;
		Ref<Texture2D> m_DefaultTexture;
		glm::vec4 m_DefaultColor;
		bool m_LineMode = false;
	};
}