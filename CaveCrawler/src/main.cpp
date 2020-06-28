#include "Engine/Square.h"

#include "Game/Random.h"
#include "Game/GameLayer.h"

class CaveCrawler : public Square::Application
{
public:
	CaveCrawler()
	{
		Random::Init();

		auto layer = new GameLayer();
		PushLayer(layer);
	}

	~CaveCrawler()
	{

	}
};

Square::Application* Square::CreateApplication()
{
	return new CaveCrawler();
}

int main(int argc, char** argv)
{
	auto app = Square::CreateApplication();
	app->Run();
	delete app;

	return EXIT_SUCCESS;
}