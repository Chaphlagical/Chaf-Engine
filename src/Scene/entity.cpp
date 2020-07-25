#include <Scene/entity.h>

namespace Chaf
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{

	}

	bool Entity::SameOf(Entity& node)
	{
		entt::entity nodeHandle;
		node.GetHandle(nodeHandle);
		return (nodeHandle == m_EntityHandle);
	}

	Entity Entity::GetFirstChild()
	{
		if (!m_Scene->m_Registry.valid(m_EntityHandle))
			return { entt::null, m_Scene };
		auto relation = m_Scene->m_Registry.get<Relationship>(m_EntityHandle);
		return { relation.first, m_Scene };
	}

	Entity Entity::GetNext()
	{
		if (!m_Scene->m_Registry.valid(m_EntityHandle))
			return { entt::null, m_Scene };
		auto relation = m_Scene->m_Registry.get<Relationship>(m_EntityHandle);
		return { relation.next, m_Scene };
	}

	Entity Entity::GetParent()
	{
		if (!m_Scene->m_Registry.valid(m_EntityHandle))
			return { entt::null, m_Scene };
		auto relation = m_Scene->m_Registry.get<Relationship>(m_EntityHandle);
		return { relation.parent, m_Scene };
	}

	Entity Entity::GetPrev()
	{
		if (!m_Scene->m_Registry.valid(m_EntityHandle))
			return { entt::null, m_Scene };
		auto relation = m_Scene->m_Registry.get<Relationship>(m_EntityHandle);
		return { relation.prev, m_Scene };
	}

	void Entity::AddChild(Entity& node)
	{
		entt::entity nodeHandle;
		node.GetHandle(nodeHandle);

		auto& relation_this = m_Scene->m_Registry.get<Relationship>(m_EntityHandle);
		auto& relation_node = m_Scene->m_Registry.get<Relationship>(nodeHandle);
		if (m_Scene->m_Registry.valid(relation_this.first))
		{
			auto& relation_first = m_Scene->m_Registry.get<Relationship>(relation_this.first);
			relation_first.prev = nodeHandle;
		}
		relation_node.next = relation_this.first;
		relation_node.parent = m_EntityHandle;
		relation_this.first = nodeHandle;
		relation_this.children++;
	}

	void Entity::AddSibling(Entity& node)
	{
		entt::entity nodeHandle;
		node.GetHandle(nodeHandle);

		auto& relation_this = m_Scene->m_Registry.get<Relationship>(m_EntityHandle);
		auto& relation_node = m_Scene->m_Registry.get<Relationship>(nodeHandle);
		if (m_Scene->m_Registry.valid(relation_this.next))
		{
			auto& relation_next = m_Scene->m_Registry.get<Relationship>(relation_this.next);
			relation_next.prev = nodeHandle;
		}
		relation_node.next = relation_this.next;
		relation_this.next = nodeHandle;
		relation_node.prev = m_EntityHandle;
		relation_node.parent = relation_this.parent;

		if (m_Scene->m_Registry.valid(relation_this.parent))
		{
			auto& relation_this_parent = m_Scene->m_Registry.get<Relationship>(relation_this.parent);
			relation_this_parent.children++;
		}
	}

	void Entity::MoveAsChildOf(Entity& dstNode)
	{
		entt::entity dstNodeHandle;
		dstNode.GetHandle(dstNodeHandle);
		auto& relation_src = m_Scene->m_Registry.get<Relationship>(m_EntityHandle);
		auto& relation_dst = m_Scene->m_Registry.get<Relationship>(dstNodeHandle);
		auto& relation_src_parent = m_Scene->m_Registry.get<Relationship>(relation_src.parent);
		
		if (dstNode.IsSonOf((*this)) || (*this).SameOf(dstNode.GetParent()))
			return;

		relation_src_parent.children--;
		relation_dst.children++;

		if (m_Scene->m_Registry.valid(relation_src.next))
		{
			auto& relation_src_next = m_Scene->m_Registry.get<Relationship>(relation_src.next);
			relation_src_next.prev = relation_src.prev;
		}
		if (m_Scene->m_Registry.valid(relation_src.prev))
		{
			auto& relation_src_prev = m_Scene->m_Registry.get<Relationship>(relation_src.prev);
			relation_src_prev.next = relation_src.next;
		}
		else
		{
			relation_src_parent.first = relation_src.next;
		}
		relation_src.next = relation_dst.first;
		relation_src.prev = entt::null;
		relation_src.parent = dstNodeHandle;
		
		if (m_Scene->m_Registry.valid(relation_dst.first))
		{
			auto& relation_dst_first = m_Scene->m_Registry.get<Relationship>(relation_dst.first);
			relation_dst_first.prev = m_EntityHandle;
		}
		relation_dst.first = m_EntityHandle;
	}

	void Entity::Remove()
	{
		auto& relation = m_Scene->m_Registry.get<Relationship>(m_EntityHandle);
		auto& relation_parent = m_Scene->m_Registry.get<Relationship>(relation.parent);
		relation_parent.children--;
		if (m_Scene->m_Registry.valid(relation.prev))
		{
			auto& relation_prev = m_Scene->m_Registry.get<Relationship>(relation.prev);
			relation_prev.next = relation.next;
		}
		else
			relation_parent.first = relation.next;
		auto& child = (*this).GetFirstChild();
		while (child)
		{
			auto tmp = child.GetNext();
			child.Remove();
			child = tmp;
		}
		m_Scene->m_Registry.destroy(m_EntityHandle);
	}

	bool Entity::IsSonOf(Entity& node)
	{
		if (!node)return false;
		if (node.SameOf(*this))
			return true;
		auto child = node.GetFirstChild();
		while (child)
		{
			if ((*this).SameOf(child))
				return true;
			child = child.GetNext();
		}
		return false;
	}

	bool Entity::HasParent()
	{
		auto& relation = m_Scene->m_Registry.get<Relationship>(m_EntityHandle);
		return (m_Scene->m_Registry.valid(relation.parent));
	}

	bool Entity::HasChild()
	{
		auto& relation = m_Scene->m_Registry.get<Relationship>(m_EntityHandle);
		return (m_Scene->m_Registry.valid(relation.first));
	}

	bool Entity::IsRoot()
	{
		return (m_Scene->m_Root == m_EntityHandle);
	}

	Entity Entity::CreateChild(const std::string& name)
	{
		return m_Scene->CreateEntity(name, m_EntityHandle);
	}
}