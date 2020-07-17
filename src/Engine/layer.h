#pragma once

#include <Engine/core.h>
#include <Engine/time.h>
#include <Engine/event_system.h>

#include <string>
#include <vector>

namespace Chaf
{
	class CHAF_API Layer
	{
	public:
		Layer(const std::string& name = "Layer")
			:m_DebugName(name) {}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep timestep) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};

	/////////////////////// Stack ///////////////////////////////

	class CHAF_API LayerStack
	{
	public:
		LayerStack() {}
		~LayerStack()
		{
			for (Layer* layer : m_Layers)
				delete layer;
		}

		void PushLayer(Layer* layer)
		{
			m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
			m_LayerInsertIndex++;
		}

		void PushOverlay(Layer* overlay) { m_Layers.emplace_back(overlay); }

		void PopLayer(Layer* layer)
		{
			auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
			if (it != m_Layers.end())
			{
				layer->OnDetach();
				m_Layers.erase(it);
				m_LayerInsertIndex--;
			}
		}

		void PopOverlay(Layer* overlay)
		{
			auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
			if (it != m_Layers.end())
			{
				overlay->OnDetach();
				m_Layers.erase(it);
			}
		}

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}