#include "Text.h"

namespace Square {

	Text::Text()
	{
		mReversed = false;
	}

	Text::Text(const std::string& text, const std::string& fontpath, int size, SDL_Color color)
	{
		if (text.size() > 0)
		{
			float width = 0;
			for (const char& c : text)
			{
				mSprites.push_back(std::shared_ptr<Sprite>(new Sprite(c, fontpath, size, color)));
				mSprites.back()->Parent(this);
				width += mSprites.back()->ScaledDimensions().x;
			}

			width /= 2.0f;
			width -= mSprites.front()->ScaledDimensions().x * 0.5f;
			for (auto& sprite : mSprites)
			{
				sprite->Pos(-VEC2_RIGHT * width);
				width -= sprite->ScaledDimensions().x;
			}

			mReversed = false;
		}
		else
			LOG(ERROR) << "Error: You cannot have a text sprite containing 0 characters!";
	}

	Vector2 Text::ScaledDimensions()
	{
		Vector2 scaledDimensions = Scale();

		int width = 0;
		int height = 0;

		for (auto sprite : mSprites)
		{
			width += sprite->ScaledDimensions().x;

			if (height < sprite->ScaledDimensions().y)
				height = sprite->ScaledDimensions().y;
		}

		scaledDimensions.x *= width;
		scaledDimensions.y *= height * 0.5f;

		return scaledDimensions;
	}

	void Text::Flip(SDL_RendererFlip flip)
	{
		for (const auto& sprite : mSprites)
			sprite->Flip(flip);

		if ((flip == SDL_FLIP_HORIZONTAL && !mReversed)
			|| flip == SDL_FLIP_NONE && mReversed)
		{
			int start = 0;
			int end = mSprites.size() - 1;

			while (start < end)
			{
				Vector2 pos = mSprites[start]->Pos(local);
				mSprites[start]->Pos(mSprites[end]->Pos(local));
				mSprites[end]->Pos(pos);
				start++;
				end--;
			}

			mReversed = !mReversed;
		}
	}

	void Text::Color(SDL_Color color)
	{
		for (const auto& sprite : mSprites)
			sprite->Color(color);
	}

	void Text::FlashColor(SDL_Color color)
	{
		for (const auto& sprite : mSprites)
			sprite->FlashColor(color);
	}

	void Text::Flash()
	{
		for (const auto& sprite : mSprites)
			sprite->Flash();
	}

	void Text::Render(bool ignoreCamera)
	{
		for (const auto& sprite : mSprites)
			sprite->Render(ignoreCamera);
	}

}