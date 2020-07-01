#include "Graphics.h"

namespace Square {

	bool Graphics::sInitialized = false;

	Graphics& Graphics::Instance()
	{
		static Graphics instance;
		return instance;
	}

	bool Graphics::Initialized()
	{
		return sInitialized;
	}

	Graphics::Graphics()
		: mWindow(nullptr, SDL_DestroyWindow), mRenderer(nullptr, SDL_DestroyRenderer)
	{
		sInitialized = Init();
	}

	Graphics::~Graphics()
	{
		sInitialized = false;

		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

	bool Graphics::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			LOG(ERROR) << "SDL Initialization Error: " << SDL_GetError();
			return false;
		}

		mWindow.reset(SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));

		if (!mWindow)
		{
			LOG(ERROR) << "Window Creation Error: " << SDL_GetError();
			return false;
		}

		mRenderer.reset(SDL_CreateRenderer(mWindow.get(), -1, SDL_RENDERER_ACCELERATED));

		if (!mRenderer)
		{
			LOG(ERROR) << "Renderer Creation Error: " << SDL_GetError();
			return false;
		}

		mDefaultDrawColor = { 0x00, 0x00, 0x00, 0xFF };
		SDL_SetRenderDrawColor(mRenderer.get(), mDefaultDrawColor.r, mDefaultDrawColor.g, mDefaultDrawColor.b, mDefaultDrawColor.a);
		SDL_SetRenderDrawBlendMode(mRenderer.get(), SDL_BLENDMODE_BLEND);

		int flags = IMG_INIT_PNG;
		if (!(IMG_Init(flags) & flags))
		{
			LOG(ERROR) << "IMG Initialization Error: " << IMG_GetError();
			return false;
		}

		if (TTF_Init() < 0)
		{
			LOG(ERROR) << "TTF Initialization Error: " << TTF_GetError();
			return false;
		}

		//SDL_RenderSetLogicalSize(mRenderer.get(), SCREEN_WIDTH * 4, SCREEN_HEIGHT * 4);
		mCamera = SDL_Rect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

		return true;
	}

	std::shared_ptr<SDL_Texture> Graphics::LoadTexture(std::string path)
	{
		std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> surface(IMG_Load(path.c_str()), SDL_FreeSurface);

		if (!surface)
		{
			LOG(ERROR) << "Image Load Error: Path " << path.c_str() << " - Error " << IMG_GetError();
			return std::shared_ptr<SDL_Texture>();
		}

		std::shared_ptr<SDL_Texture> tex(SDL_CreateTextureFromSurface(mRenderer.get(), surface.get()), SDL_DestroyTexture);
		if (!tex)
		{
			LOG(ERROR) << "Create Texture Error: " << SDL_GetError();
			return std::shared_ptr<SDL_Texture>();
		}

		return tex;
	}

	std::shared_ptr<SDL_Texture> Graphics::CreateTextTexture(std::weak_ptr<TTF_Font> font, const std::string& text, SDL_Color color)
	{
		std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> surface(TTF_RenderText_Solid(font.lock().get(), text.c_str(), color), SDL_FreeSurface);
		if (!surface)
		{
			LOG(ERROR) << "Text Render Error: " << TTF_GetError();
			return std::shared_ptr<SDL_Texture>();
		}

		std::shared_ptr<SDL_Texture> tex(SDL_CreateTextureFromSurface(mRenderer.get(), surface.get()), SDL_DestroyTexture);
		if (!tex)
		{
			LOG(ERROR) << "Text Texture Creation Error: " << SDL_GetError();
			return std::shared_ptr<SDL_Texture>();
		}

		return tex;
	}

	void Graphics::ClearBackBuffer()
	{
		SDL_RenderClear(mRenderer.get());
	}

	void Graphics::DrawTexture(std::weak_ptr<SDL_Texture> tex, const SDL_Rect* clip, SDL_Rect* rend, float angle, SDL_RendererFlip flip, bool ignoreCamera)
	{
		if (!ignoreCamera)
		{
			Vector2 size = RotateVector(Vector2(rend->w, rend->h), angle);
			size.x = abs(size.x);
			size.y = abs(size.y);

			if (rend->x + size.x < mCamera.x) //Left of camera
				return;
			else if (rend->x > mCamera.x + mCamera.w) //Right of camera
				return;
			else if (rend->y + size.y < mCamera.y) //Above camera
				return;
			else if (rend->y > mCamera.y + mCamera.h) //Below camera
				return;

			rend->x -= mCamera.x + mOffset.x;
			rend->y -= mCamera.y + mOffset.y;
		}

		SDL_RenderCopyEx(mRenderer.get(), tex.lock().get(), clip, rend, angle, nullptr, flip);
	}

	void Graphics::DrawPixel(Vector2 pos, SDL_Color color)
	{
		SDL_SetRenderDrawColor(mRenderer.get(), color.r, color.g, color.b, color.a);
		SDL_RenderDrawPoint(mRenderer.get(), pos.x, pos.y);
		SDL_SetRenderDrawColor(mRenderer.get(), mDefaultDrawColor.r, mDefaultDrawColor.g, mDefaultDrawColor.b, mDefaultDrawColor.a);
	}

	void Graphics::DrawRectangle(Vector2 pos, int width, int height, SDL_Color color)
	{
		SDL_Rect rectangle;
		rectangle.x = pos.x;
		rectangle.y = pos.y;
		rectangle.w = width;
		rectangle.h = height;

		SDL_SetRenderDrawColor(mRenderer.get(), color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(mRenderer.get(), &rectangle);
		SDL_SetRenderDrawColor(mRenderer.get(), mDefaultDrawColor.r, mDefaultDrawColor.g, mDefaultDrawColor.b, mDefaultDrawColor.a);
	}

	void Graphics::SetLimit(Vector2 vec)
	{
		mLimit = vec;
	}

	void Graphics::Camera(Vector2 vec)
	{
		mCamera.x = vec.x - mCamera.w * 0.5f;
		mCamera.y = vec.y - mCamera.h * 0.5f;

		if (mCamera.x < 0.0f)
			mCamera.x = 0.0f;
		else if (mCamera.x + mCamera.w > mLimit.x)
			mCamera.x = mLimit.x - mCamera.w;

		if (mCamera.y <= 0)
			mCamera.y = 0.0f;
		else if (mCamera.y + mCamera.h > mLimit.y)
			mCamera.y = mLimit.y - mCamera.h;
	}

	void Graphics::CameraShake()
	{
		mOffset.x += rand() % 4 + 1;
		mOffset.y += rand() % 4 + 1;
	}

	void Graphics::Render()
	{
		SDL_RenderPresent(mRenderer.get());
		mOffset = VEC2_ZERO;
	}

}