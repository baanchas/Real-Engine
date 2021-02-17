#include "repch.h"

#include "Application.h"

int main()
{
	RealEngine::Log::InitLog();

	RealEngine::Application::Get()->Run();
  
	return 0;
}