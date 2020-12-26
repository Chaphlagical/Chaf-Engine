#include <Editor/application.h>
#include <Scene/maincamera_layer.h>
#include <Scene/scene_layer.h>
#include <Editor/editor_layer.h>
#include "app.h"
#include <Canvas/CanvasLayer.h>

namespace Chaf
{
	class App : public Application
	{
	public:
		App()
			:Application("Chaf Engine")
		{
			PushLayer(new MainCameraLayer());
			PushLayer(new SceneLayer());
			PushLayer(new EditorLayer());
			PushLayer(new CanvasLayer());
			PushLayer(new AppLayer());
		}

		~App()
		{

		}

	};

	Application* Create()
	{
		return new App();
	}
}