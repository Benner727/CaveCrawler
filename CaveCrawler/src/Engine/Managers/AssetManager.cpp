#include "AssetManager.h"

namespace Square {

	AssetManager& AssetManager::Instance()
	{
		static AssetManager instance;
		return instance;
	}

	AssetManager::AssetManager()
		: mBasePath(std::make_unique<std::string>(SDL_GetBasePath()))
	{

	}

	AssetManager::~AssetManager()
	{

	}

	std::weak_ptr<TTF_Font> AssetManager::GetFont(const std::string& filename, int size)
	{
		std::string fullPath = GetBasePath();
		fullPath.append("Assets/" + filename);
		std::string key = fullPath + (char)size;

		if (!mFonts[key])
		{
			mFonts[key] = std::shared_ptr<TTF_Font>(TTF_OpenFont(fullPath.c_str(), size), TTF_CloseFont);
			if (!mFonts[key])
				LOG(ERROR) << "Font Loading Error: Font- " << filename << " Error- " << TTF_GetError();
		}

		return mFonts[key];
	}

	std::shared_ptr<SDL_Texture> AssetManager::GetTexture(const std::string& filename)
	{
		std::string fullPath = GetBasePath();
		fullPath.append("Assets/" + filename);

		if (!mTextures[fullPath])
			mTextures[fullPath] = Graphics::Instance().LoadTexture(fullPath);

		return mTextures[fullPath];
	}

	std::shared_ptr<SDL_Texture> AssetManager::GetText(const std::string& text, const std::string& filename, int size, SDL_Color color)
	{
		std::shared_ptr<TTF_Font> font(GetFont(filename, size));

		std::string key = text + filename + (char)size + (char)color.r + (char)color.b + (char)color.g;

		if (!mText[key])
			mText[key] = Graphics::Instance().CreateTextTexture(font, text, color);

		return mText[key];
	}

	std::shared_ptr<Mix_Music> AssetManager::GetMusic(const std::string& filename)
	{
		std::string fullPath = GetBasePath();
		fullPath.append("Assets/" + filename);

		if (!mMusic[fullPath])
		{
			mMusic[fullPath] = std::shared_ptr<Mix_Music>(Mix_LoadMUS(fullPath.c_str()), Mix_FreeMusic);
			if (!mMusic[fullPath])
				LOG(ERROR) << "Music Loading Error: File-" << filename << " Erorr-" << Mix_GetError();
		}

		return mMusic[fullPath];
	}

	std::shared_ptr<Mix_Chunk> AssetManager::GetSFX(const std::string& filename)
	{
		std::string fullPath = GetBasePath();
		fullPath.append("Assets/" + filename);

		if (!mSFX[fullPath])
		{
			mSFX[fullPath] = std::shared_ptr<Mix_Chunk>(Mix_LoadWAV(fullPath.c_str()), Mix_FreeChunk);
			if (!mSFX[fullPath])
				LOG(ERROR) << "SFX Loading Error: File-" << filename << " Erorr-" << Mix_GetError();
		}

		return mSFX[fullPath];
	}

}