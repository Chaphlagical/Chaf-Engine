#pragma once

#include <Engine/log.h>
#include <Engine/core.h>
#include <Scene/scene.h>

namespace Chaf
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			CHAF_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			CHAF_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		template<typename T>
		T& GetComponent()
		{
			CHAF_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		operator bool() const 
		{
			if (m_Scene == nullptr)return false;
			return m_Scene->m_Registry.valid(m_EntityHandle); 
		}

		uint32_t ID() { return static_cast<uint32_t>(m_EntityHandle); }

		void GetHandle(entt::entity& node) { node = m_EntityHandle; }
		bool SameOf(Entity& node);	//	not including same scene;
		bool HasParent();
		bool HasChild();
		bool IsRoot();

		Entity GetFirstChild();
		Entity GetNext();
		Entity GetParent();
		Entity GetPrev();
		void AddChild(Entity& node);
		void AddSibling(Entity& node);
		void MoveAsChildOf(Entity& dstNode);	//	this -> disNode
		void Remove();
		Entity CreateChild(const std::string& name = std::string());
	private:
		bool IsSonOf(Entity& node);
	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}