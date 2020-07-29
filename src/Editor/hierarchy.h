#pragma once

#include <Engine/core.h>
#include <Scene/entity.h>

namespace Chaf
{

	class CHAF_API Hierachy
	{
	public:
		static void ShowHierarchy(bool* p_open);
		static void RecurseTree(Entity node);

		static void RemoveEntity(Entity& node);

	};
}