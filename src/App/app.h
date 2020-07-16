#include <Editor/application.h>

namespace Chaf
{
	class App: public Application
	{
	public:
		App()
			:Application("Chaf Engine")
		{

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