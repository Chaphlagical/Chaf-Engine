#pragma

#include <string>

#include <Engine/core.h>

namespace Chaf
{
	class CHAF_API Application
	{
	public:
		Application(const std::string name = "Chaf Engine");
		virtual ~Application();

		void Run();
	private:
		bool m_Running = true;
	};

	//	implement in client
	static Application* Create();
}