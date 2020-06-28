#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <iostream>
#include <string>

#include "Engine/Utilities/MathHelper.h"
#include "Engine/Utilities/Logger.h"

namespace Square {

	class Graphics
	{
	public:
		static const int SCREEN_WIDTH = 768;
		static const int SCREEN_HEIGHT = 512;
		const char* WINDOW_TITLE = "Cave Crawler";

	private:
		static bool sInitialized;

		std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> mWindow;
		std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> mRenderer;

		SDL_Color mDefaultDrawColor;
		SDL_Rect mCamera;
		Vector2 mLimit;
		Vector2 mOffset;

		Graphics();
		~Graphics();

		bool Init();

	public:
		static Graphics& Instance();
		static bool Initialized();

		std::shared_ptr<SDL_Texture> LoadTexture(std::string path);
		std::shared_ptr<SDL_Texture> CreateTextTexture(std::weak_ptr<TTF_Font> font, const std::string& text, SDL_Color color);

		void ClearBackBuffer();

		void DrawTexture(std::weak_ptr<SDL_Texture> tex, const SDL_Rect* clip = nullptr, SDL_Rect* rend = nullptr, float angle = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE, bool ignoreCamera = false);
		void DrawPixel(Vector2 pos, SDL_Color color);
		void DrawRectangle(Vector2 pos, int width, int height, SDL_Color color);

		void SetLimit(Vector2 vec);
		void Camera(Vector2 vec);
		inline const SDL_Rect& Camera() { return mCamera; }
		void CameraShake();

		void Render();
	};

}