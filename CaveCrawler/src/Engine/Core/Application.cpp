#include "Application.h"

namespace Square {

	Application::Application()
		: mEngine(Engine::Instance())
	{

	}


	void Application::PushLayer(Layer* layer)
	{
		mEngine.PushLayer(layer);
	}

	void Application::PopLayer(Layer* layer)
	{
		mEngine.PopLayer(layer);
	}

	void Application::Run()
	{
		mEngine.Run();
	}

}