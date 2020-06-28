#include "AnimatedSprite.h"

namespace Square {

	AnimatedSprite::AnimatedSprite()
		: Sprite(), mTimer(Timer::Instance())
	{
		mStartX = 0;
		mStartY = 0;

		mCurrentFrame = 0;
		mFrameCount = 0;
		mAnimationSpeed = 0.0f;
		mTimePerFrame = 0.0f;
		mAnimationTimer = 0.0f;

		mAnimationDirection = ANIM_DIR::horizontal;

		mAnimationDone = true;
		mPaused = false;

		mWrapMode = loop;
	}

	AnimatedSprite::AnimatedSprite(const std::string& filename, int x, int y, int w, int h, int frameCount, float animationSpeed, ANIM_DIR animationDir)
		: Sprite(filename, x, y, w, h), mTimer(Timer::Instance())
	{
		mStartX = x;
		mStartY = y;

		mCurrentFrame = 0;
		mFrameCount = frameCount;
		mAnimationSpeed = animationSpeed;
		mTimePerFrame = mAnimationSpeed / mFrameCount;
		mAnimationTimer = 0.0f;

		mAnimationDirection = animationDir;

		mAnimationDone = false;
		mPaused = false;

		mWrapMode = loop;
	}

	AnimatedSprite::~AnimatedSprite()
	{

	}

	void AnimatedSprite::WrapMode(WRAP_MODE mode)
	{
		mWrapMode = mode;
	}

	void AnimatedSprite::Play()
	{
		mPaused = false;
	}

	void AnimatedSprite::Pause()
	{
		mPaused = true;
	}

	void AnimatedSprite::Reset()
	{
		mAnimationTimer = 0.0f;
		mAnimationDone = false;
	}

	void AnimatedSprite::Frame(int frame)
	{
		if (frame < 0 && frame >= mFrameCount)
			return;

		if (mCurrentFrame != frame)
		{
			mCurrentFrame = frame;
			mAnimationTimer = mCurrentFrame * mTimePerFrame;
		}
	}

	bool AnimatedSprite::IsAnimating() const
	{
		return !mAnimationDone;
	}

	void AnimatedSprite::Update()
	{
		if (!mAnimationDone)
		{
			if (!mPaused)
				mAnimationTimer += mTimer.DeltaTime();

			if (mAnimationTimer >= mAnimationSpeed)
			{
				if (mWrapMode == loop)
				{
					mAnimationTimer -= mAnimationSpeed;
				}
				else
				{
					mAnimationDone = true;
					mAnimationTimer = mAnimationSpeed - mTimePerFrame;
				}
			}

			mCurrentFrame = (int)(mAnimationTimer / mTimePerFrame);

			if (mAnimationDirection == horizontal)
			{
				mClipRect.x = mStartX + mCurrentFrame * mWidth;
			}
			else
			{
				mClipRect.y = mStartY + mCurrentFrame * mHeight;
			}
		}
	}

}