#pragma once

#include <Engine/core.h>
#include <Scene/primitive.h>
#include <glm/glm.hpp>

namespace Chaf
{
	class CHAF_API Menu
	{
	public:
		Menu() {}
		~Menu() = default;

		static void ShowMainMenu();
	private:
		static void AddObject();
		static void Reset();
	private:
		static bool m_AddObjectEvent;
		static MeshType m_NewObjectType;
		static int m_Sample;
		static glm::vec3 m_NewPosition;
		static glm::vec3 m_NewRotation;
		static glm::vec3 m_NewScale;
	};
}