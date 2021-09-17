#include <Editor/basic.h>
#include <Renderer/mesh.h>
#include <Scene/components.h>
#include <Scene/scene_layer.h>
#include <Editor/FileDialog/ImFileDialog.h>
#include <imgui.h>

#include <glad/glad.h>

namespace Chaf
{
	Entity EditorBasic::m_SelectEntity = Entity();
	std::string EditorBasic::m_PopupFlag = "";
	bool EditorBasic::m_FlagShowHierarchy = true;
	bool EditorBasic::m_FlagShowInspector = true;
	bool	EditorBasic::m_FlagDemoWindow = false;
	bool EditorBasic::m_FlagStyleEditor = false;
	bool EditorBasic::m_FlagTerminal = true;
	bool EditorBasic::m_InitFileDialog = false;

	ImGuizmo::OPERATION EditorBasic::mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE EditorBasic::mCurrentGizmoMode = ImGuizmo::WORLD;

	void EditorBasic::AddObjectMenu(Entity& entity)
	{
		if (ImGui::BeginMenu("New Object"))
		{
			for (auto item : MeshType2Item)
				if (ImGui::MenuItem(item.second.c_str()))
				{
					m_PopupFlag = item.second;
					SetSelectEntity(entity);
				}
			ImGui::EndMenu();
		}
	}

	void EditorBasic::GetFileDialog(const std::string& label, const std::string& format, std::function<void(const std::string&)> func)
	{
		if (!m_InitFileDialog)
		{
			ifd::FileDialog::Instance().CreateTexture = [](uint8_t* data, int w, int h, char fmt) -> void* {
				GLuint tex;

				glGenTextures(1, &tex);
				glBindTexture(GL_TEXTURE_2D, tex);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, (fmt == 0) ? GL_BGRA : GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);

				return (void*)tex;
			};
			ifd::FileDialog::Instance().DeleteTexture = [](void* tex) {
				GLuint texID = (GLuint)tex;
				glDeleteTextures(1, &texID);
			};

			m_InitFileDialog = true;
		}

		if (m_PopupFlag == label)
			ifd::FileDialog::Instance().Open(label, "Open a file",  format + "{" + format + "}, .*");

		if (ifd::FileDialog::Instance().IsDone(label)) {
			if (ifd::FileDialog::Instance().HasResult()) {
				std::string path = ifd::FileDialog::Instance().GetResult().u8string();
				func(path);
			}
			ifd::FileDialog::Instance().Close();
			m_PopupFlag = "";
		}
	}

	void EditorBasic::Rename(Entity& entity)
	{
		ImGui::Text("Rename");
		char buf[50];
		std::string tmp = entity.GetComponent<TagComponent>().Tag;
		std::string logstr = "Rename " + tmp + " to ";
		for (int i = 0; i < tmp.length(); i++)
			buf[i] = tmp[i];
		buf[tmp.length()] = '\0';
		ImGui::InputText("##edit", buf, 50);
		entity.GetComponent<TagComponent>().Tag = buf;
		if (ImGui::Button("OK"))
		{
			ImGui::CloseCurrentPopup();
			logstr += buf;
			CHAF_INFO(logstr);
		}

	}

	void EditorBasic::AddObjectAnswer()
	{
		if (m_PopupFlag != MeshType2Item[MeshType::Model] && MeshItem2Type.count(m_PopupFlag) > 0)
		{
			ImGui::Begin(m_PopupFlag.c_str());

			ImGui::Text("Transform");
			static glm::vec3 position{ 0.0f };
			static glm::vec3 rotation{ 0.0f };
			static glm::vec3 scale{ 1.0f };
			ImGui::DragFloat3("Position", (float*)&position, 0.1f);
			ImGui::DragFloat3("Rotation", (float*)&rotation, 1.0f);
			ImGui::DragFloat3("Scale", (float*)&scale, 0.01f);

			static int sample = 1;
			if (MeshItem2Type[m_PopupFlag] != MeshType::None)
			{
				ImGui::Separator();
				ImGui::Text("Sample");
				ImGui::SliderInt("sample", &sample, 1, 30);
			}
			ImGui::Separator();
			ImGui::Text("Name");
			static char buf[32] = "New Object";
			ImGui::InputText("##edit", buf, 50);

			ImGui::Separator();

			if (ImGui::Button("Create"))
			{
				auto entity = m_SelectEntity.CreateChild(std::string(buf));
				if (MeshItem2Type[m_PopupFlag] != MeshType::None)
					entity.AddComponent<MeshComponent>(MeshItem2Type[m_PopupFlag], sample);
				entity.GetComponent<TransformComponent>().Position = position;
				entity.GetComponent<TransformComponent>().Rotation = rotation;
				entity.GetComponent<TransformComponent>().Scale = scale;
				entity.GetComponent<TransformComponent>().Update();
				m_PopupFlag = "";
				m_SelectEntity = entity;
				CHAF_INFO("Create new object: " + entity.GetComponent<TagComponent>().Tag);
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
				m_PopupFlag = "";
			}
			ImGui::End();
		}

		GetFileDialog("Model", ".obj", [](const std::string& path)
			{
				auto entity = m_SelectEntity.CreateChild(std::string("mesh"));
				entity.AddComponent<MeshComponent>(path);
				m_PopupFlag = "";
				m_SelectEntity = entity;
				CHAF_INFO("Create new object: " + entity.GetComponent<TagComponent>().Tag);
			});
	}

	void EditorBasic::ShowTexture(const char* label, const Ref<Texture2D>& texture, std::function<void(void)> func)
	{
		auto textureID = texture->HasImage() ? texture->GetRendererID() : SceneLayer::GetDefaultRenderData()->checkboardTexture->GetRendererID();
		ImGui::Image((void*)textureID, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			ImGui::OpenPopup(label);
		if (ImGui::BeginPopup(label))
		{
			ImGui::Image((void*)textureID, ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
			func();
			ImGui::EndPopup();
		}
	}
}