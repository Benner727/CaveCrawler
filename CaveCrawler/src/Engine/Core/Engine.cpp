#include "Engine.h"

namespace Square {

	Engine& Engine::Instance()
	{
		static Engine instance;
		return instance;
	}

	Engine::Engine()
		: mGraphics(Graphics::Instance()), mAssetManager(AssetManager::Instance()),
		mAudio(Audio::Instance()), mInputHandler(InputHandler::Instance()),
		mTimer(Timer::Instance()), mPhysicsManager(PhysicsManager::Instance())
	{
		mQuit = !mGraphics.Initialized();

		mPhysicsManager.SetLayerCollisionMask(PhysEntity::CollisionLayers::Friendly, PhysEntity::CollisionFlags::Hostile | PhysEntity::CollisionFlags::HostileProjectiles | PhysEntity::CollisionFlags::PowerUp);
		mPhysicsManager.SetLayerCollisionMask(PhysEntity::CollisionLayers::FriendlyProjectiles, PhysEntity::CollisionFlags::Hostile);
		mPhysicsManager.SetLayerCollisionMask(PhysEntity::CollisionLayers::Hostile, PhysEntity::CollisionFlags::Friendly | PhysEntity::CollisionFlags::FriendlyProjectiles);
		mPhysicsManager.SetLayerCollisionMask(PhysEntity::CollisionLayers::HostileProjectiles, PhysEntity::CollisionFlags::Friendly);
		mPhysicsManager.SetLayerCollisionMask(PhysEntity::CollisionLayers::PowerUp, PhysEntity::CollisionFlags::Friendly);
	}

	Engine::~Engine()
	{
		for (Layer* layer : mLayerStack)
			delete layer;
	}


	void Engine::PushLayer(Layer* layer)
	{
		mLayerStack.emplace_back(layer);
	}

	void Engine::PopLayer(Layer* layer)
	{
		auto it = std::find(mLayerStack.begin(), mLayerStack.end(), layer);
		if (it != mLayerStack.end())
		{
			mLayerStack.erase(it);
		}
	}

	void Engine::EarlyUpdate()
	{
		mTimer.Update();

		mInputHandler.Update();
	}

	void Engine::Update()
	{
		//Update game here
		for (auto& layer : mLayerStack)
			layer->OnUpdate();
	}

	void Engine::LateUpdate()
	{
		mPhysicsManager.Update();

		mInputHandler.UpdatePrevInput();
	}

	void Engine::Render()
	{
		mGraphics.ClearBackBuffer();

		//Render game here
		for (auto& layer : mLayerStack)
			layer->OnRender();

		mGraphics.Render();
	}

	void Engine::Run()
	{
		while (!mQuit)
		{
			EarlyUpdate();

			while (SDL_PollEvent(&mEvent) != 0)
			{
				if (mEvent.type == SDL_QUIT)
					mQuit = true;
				else if (mEvent.type == SDL_KEYDOWN)
					mInputHandler.LastKey(mEvent.key.keysym.scancode);
			}

			if (mTimer.DeltaTime() >= 1.0f / FRAME_RATE)
			{
				Update();
				LateUpdate();
				Render();

				mTimer.Reset();
			}
		}
	}

}