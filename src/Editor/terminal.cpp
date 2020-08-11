#include <Editor/terminal.h>
#include <fstream>
#include <string>
#include <imgui.h>

namespace Chaf
{
	void Terminal::LoggingConsole(bool* handle)
	{
		ImGui::Begin("console", handle);
		std::fstream file;
		file.open("log.txt");

		std::string s;
		for (int i = 0; std::getline(file, s); i++)
		{
			if (i >= 4)
				ImGui::Text(s.data());
		}
		for (int i = 0; i < 4; i++)
			ImGui::Text("");
		file.close();
		ImGui::End();
	}
}