#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <string>

#include "Engine/Utilities/MathHelper.h"

namespace Square {

	class InputHandler
	{
	public:
		enum MOUSE_BUTTON { left = 0, right, middle, back, forward };

	private:
		Uint8* mPrevKeyboardState;
		const Uint8* mKeyboardState;
		int mKeyLength;

		Uint32 mPrevMouseState;
		Uint32 mMouseState;

		int mMouseXPos;
		int mMouseYPos;

		SDL_Scancode mLastKey;

		InputHandler();
		~InputHandler();

	public:
		static InputHandler& Instance();

		bool KeyDown(const SDL_Scancode& scanCode);
		bool KeyPressed(const SDL_Scancode& scanCode);
		bool KeyReleased(const SDL_Scancode& scanCode);

		bool MouseButtonDown(const MOUSE_BUTTON& button);
		bool MouseButtonPressed(const MOUSE_BUTTON& button);
		bool MouseButtonReleased(const MOUSE_BUTTON& button);

		Vector2 MousePos();

		void LastKey(const SDL_Scancode& scanCode);
		const SDL_Scancode& LastKey();

		void Update();
		void UpdatePrevInput();
	};

}