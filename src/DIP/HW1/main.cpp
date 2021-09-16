#include <DIP/DIP/DIP.h>
#include "DIPLayer.h"

int main()
{
	Chaf::Log::Init();

	auto app = Chaf::Create();

	app->PushLayer(new Chaf::DIPLayer);

	app->Run();

	return 0;
}