#include <CAGD/CAGD/CAGD.h>

#include "CAGDLayer.h"

int main()
{
	Chaf::Log::Init();

	auto app = Chaf::Create();

	app->PushLayer(new Chaf::CAGDLayer);

	app->Run();

	return 0;
}