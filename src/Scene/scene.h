#pragma once

#include <entt.hpp>
#include <Engine/core.h>
#include <Engine/time.h>
#include <Renderer/shader.h>
#include <Renderer/texture.h>
#include <Renderer/camera.h>
#include <Renderer/effect.h>

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

		void RenderObject(Camera& camera, const Ref<Cubemap>& envMap);
		void RenderLight(const Camera& camera);

		Entity GetRoot();
		bool Empty() { return (m_Root == entt::null); }
		void SetLineMode(const bool& enable) { m_LineMode = enable; }
		bool GetLineMode() { return m_LineMode; }

		entt::registry m_Registry;

	private:
		entt::entity m_Root = entt::null;
		Ref<Texture2D> m_DefaultTexture;
		Ref<Shader> m_DefaultShader;
		glm::vec4 m_DefaultColor;
		bool m_LineMode = false;
	};
}