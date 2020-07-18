#pragma

#include <string>

#include <Engine/core.h>
#include <Engine/layer.h>
#include <Renderer/window.h>
#include <Gui/imgui_layer.h>


namespace Chaf
{
	class CHAF_API Application
	{
	public:
		Application(const std::string name = "Chaf Engine");
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void Close();
	public:
		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		bool m_Running = true;
		bool m_Minimized = false;
		float m_LastFrameTime = 0.0f;
	private:
		Ref<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

	//	implement in client
	static Application* Create();
}