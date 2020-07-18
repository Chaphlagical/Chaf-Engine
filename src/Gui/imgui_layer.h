#pragma once

#include <Engine/layer.h>
#include <Engine/event_system.h>
#include <Renderer/window.h>

namespace Chaf
{
	class CHAF_API ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer(Ref<Window> window);
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();

		void SetBlockEvents(bool block) { m_BlockEvents = block; }

	private:
		Ref<Window> m_Window;
		bool m_BlockEvents = true;
		float m_Time = 0;
	};
}