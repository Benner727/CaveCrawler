#pragma once

namespace Square {

	class Layer
	{
	public:
		Layer()
		{

		}

		virtual ~Layer() = default;

		virtual void OnUpdate() {}
		virtual void OnRender() {}
	};

}