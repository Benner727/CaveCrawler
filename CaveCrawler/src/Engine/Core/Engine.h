#pragma once

#include "Engine/Managers/Audio.h"
#include "Engine/Managers/InputHandler.h"
#include "Engine/Utilities/Timer.h"
#include "Engine/Core/Layer.h"

#include <vector>

namespace Square {

	class Engine
	{
	private:
		const int FRAME_RATE = 60;

		bool mQuit;

		Graphics& mGraphics;
		AssetManager& mAssetManager;
		Audio& mAudio;
		InputHandler& mInputHandler;

		Timer& mTimer;

		std::vector<Layer*> mLayerStack;

		SDL_Event mEvent;

		Engine();
		~Engine();

		void EarlyUpdate();
		void Update();
		void LateUpdate();

		void Render();

	public:
		static Engine& Instance();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		void Run();
	};

}