#pragma once

#include <Editor/application.h>

#include <opencv2/opencv.hpp>

#include <Engine/core.h>

#include <Renderer/texture.h>

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
