#pragma once

#include "Engine/Entities/GameObject.h"
#include "Engine/Managers/AssetManager.h"

namespace Square {

	class Sprite : public GameObject
	{
	protected:
		static const int FLASH_FRAMES = 6;

		std::shared_ptr<SDL_Texture> mTexture;

		Graphics& mGraphics;

		int mWidth;
		int mHeight;

		bool mClipped;
		SDL_Rect mRenderRect;
		SDL_Rect mClipRect;

		SDL_RendererFlip mFlip;
		SDL_Color mColor;

		bool mFlash;
		SDL_Color mFlashColor;
		int mFlashFrames;

	public:
		Sprite();
		Sprite(const std::string& filename);
		Sprite(const std::string& filename, int x, int y, int w, int h);
		Sprite(const char& c, const std::string& fontpath, int size, SDL_Color color);
		
		virtual ~Sprite() = default;

		Vector2 ScaledDimensions();

		void Flip(SDL_RendererFlip flip);
		void Color(SDL_Color color);
		SDL_Color Color();

		void FlashColor(SDL_Color color);
		void Flash();

		virtual void Render(bool ignoreCamera = false);
	};

}