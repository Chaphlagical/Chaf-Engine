#pragma once

#include <Editor/application.h>

#include <Renderer/texture.h>

#include <DIP/DIP/Image.h>

namespace Chaf
{
	class DIPLayer :public Layer
	{
	public:
		DIPLayer();
		virtual ~DIPLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep timestep) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event & event) override;

	private:
		void show_texture(Ref<Image>& image, const char* label, bool file_dialog);

	private:
		Ref<Image> m_source;
		Ref<Image> m_target;
		Ref<Image> m_result;
	};
}