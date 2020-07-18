#include <Editor/application.h>
#include "app.h"

namespace Chaf
{
	class App : public Application
	{
	public:
		App()
			:Application("Chaf Engine")
		{
			PushLayer(new EditorLayer());
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