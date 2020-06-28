#include "Sprite.h"

namespace Square {

	Sprite::Sprite()
		: mGraphics(Graphics::Instance())
	{
		mTexture = nullptr;

		mClipped = false;

		mWidth = 0;
		mHeight = 0;

		mRenderRect.w = mWidth;
		mRenderRect.h = mHeight;

		mFlip = SDL_FLIP_NONE;
		mColor = { 255, 255, 255, 255 };

		mFlash = false;
		mFlashColor = { 0, 0, 0, 255 };
		mFlashFrames = 0;
	}

	Sprite::Sprite(const std::string& filename)
		: mGraphics(Graphics::Instance())
	{
		mTexture = AssetManager::Instance().GetTexture(filename);

		if (mTexture)
		{
			SDL_QueryTexture(mTexture.get(), nullptr, nullptr, &mWidth, &mHeight);
			SDL_SetTextureBlendMode(mTexture.get(), SDL_BLENDMODE_BLEND);
		}
		else
		{
			mWidth = 0;
			mHeight = 0;
		}

		mClipped = false;

		mRenderRect.w = mWidth;
		mRenderRect.h = mHeight;

		mFlip = SDL_FLIP_NONE;
		mColor = { 255, 255, 255, 255 };

		mFlash = false;
		mFlashColor = { 0, 0, 0, 255 };
		mFlashFrames = 0;
	}

	Sprite::Sprite(const std::string& filename, int x, int y, int w, int h)
		: mGraphics(Graphics::Instance())
	{
		mTexture = AssetManager::Instance().GetTexture(filename);

		if (mTexture)
		{
			SDL_SetTextureBlendMode(mTexture.get(), SDL_BLENDMODE_BLEND);
		}

		mClipped = true;

		mWidth = w;
		mHeight = h;

		mRenderRect.w = mWidth;
		mRenderRect.h = mHeight;

		mClipRect.x = x;
		mClipRect.y = y;
		mClipRect.w = mWidth;
		mClipRect.h = mHeight;

		mFlip = SDL_FLIP_NONE;
		mColor = { 255, 255, 255, 255 };

		mFlash = false;
		mFlashColor = { 0, 0, 0, 255 };
		mFlashFrames = 0;
	}

	Sprite::Sprite(const char& c, const std::string& fontpath, int size, SDL_Color color)
		: mGraphics(Graphics::Instance())
	{
		mTexture = AssetManager::Instance().GetText(std::string(1, c), fontpath, size, { 255, 255, 255 });

		if (mTexture)
		{
			SDL_QueryTexture(mTexture.get(), nullptr, nullptr, &mWidth, &mHeight);
			SDL_SetTextureBlendMode(mTexture.get(), SDL_BLENDMODE_BLEND);
		}
		else
		{
			mWidth = 0;
			mHeight = 0;
		}

		mClipped = false;

		mRenderRect.w = mWidth;
		mRenderRect.h = mHeight;

		mFlip = SDL_FLIP_NONE;
		mColor = color;

		mFlash = false;
		mFlashColor = { 0, 0, 0, 255 };
		mFlashFrames = 0;
	}

	Vector2 Sprite::ScaledDimensions()
	{
		Vector2 scaledDimensions = Scale();
		scaledDimensions.x *= mWidth;
		scaledDimensions.y *= mHeight;

		return scaledDimensions;
	}

	void Sprite::Flip(SDL_RendererFlip flip)
	{
		mFlip = flip;
	}

	void Sprite::Color(SDL_Color color)
	{
		mColor = color;
	}

	SDL_Color Sprite::Color()
	{
		return mColor;
	}

	void Sprite::FlashColor(SDL_Color color)
	{
		mFlashColor = color;
	}

	void Sprite::Flash()
	{
		mFlash = true;
	}

	void Sprite::Render(bool ignoreCamera)
	{
		if (mTexture)
		{
			Vector2 pos = Pos(world);
			Vector2 scale = Scale(world);

			mRenderRect.x = (int)(pos.x - mWidth * scale.x * 0.5f);
			mRenderRect.y = (int)(pos.y - mHeight * scale.y * 0.5f);

			mRenderRect.w = (int)(mWidth * scale.x);
			mRenderRect.h = (int)(mHeight * scale.y);

			if (mFlash && mFlashFrames >= 0)
			{
				SDL_SetTextureColorMod(mTexture.get(), mFlashColor.r, mFlashColor.g, mFlashColor.b);
				SDL_SetTextureAlphaMod(mTexture.get(), mFlashColor.a);
				mFlashFrames++;
			}
			else
			{
				SDL_SetTextureColorMod(mTexture.get(), mColor.r, mColor.g, mColor.b);
				SDL_SetTextureAlphaMod(mTexture.get(), mColor.a);
				if (mFlashFrames < 0) mFlashFrames++;
			}

			mGraphics.DrawTexture(mTexture, (mClipped) ? &mClipRect : nullptr, &mRenderRect, Rotation(world), mFlip, ignoreCamera);

			if (mFlash && mFlashFrames >= FLASH_FRAMES)
			{
				mFlash = false;
				mFlashFrames = -FLASH_FRAMES * 2.5f;
			}

			SDL_SetTextureColorMod(mTexture.get(), 255, 255, 255);
		}
	}

}