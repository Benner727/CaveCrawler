#pragma once

#include "Engine/Core/Engine.h"

namespace Square {

	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		void Run();

	private:
		Engine& mEngine;
	};

	//To be defined in CLIENT
	Application* CreateApplication();

}