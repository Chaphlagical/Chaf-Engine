#pragma once

#include <Editor/application.h>
#include <Editor/editor_layer.h>
#include <Scene/maincamera_layer.h>
#include <Scene/scene_layer.h>

namespace Chaf
{
	class App : public Application
	{
	public:
		App()
			:Application("Computer Aided Geometric Design Homework")
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