#include <Engine/log.h>
#include "app_init.h"

int main()
{
	Chaf::Log::Init();

	auto app = Chaf::Create();

	app->Run();

	delete app;

	CHAF_CORE_INFO("Hello World");
	return 0;
}