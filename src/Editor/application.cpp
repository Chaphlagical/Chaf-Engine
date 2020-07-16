#include <Editor/application.h>
#include <Engine/log.h>

namespace Chaf
{
	Application::Application(const std::string name)
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running)
		{
			CHAF_INFO("App running!");
		}
	}

}