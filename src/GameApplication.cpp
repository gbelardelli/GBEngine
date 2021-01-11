#include "GameApplication.h"
#include "gbe_basewindow.h"

GameApplication::GameApplication() : GBEBaseApplication("My First Game")
{
}

GameApplication::~GameApplication()
{
}

bool GameApplication::onInitialize()
{
	mMainWindow->setSize(1920, 1080);
	return true;
}

void GameApplication::onFinalize(void)
{
}

void GameApplication::onUpdate(float fElapsedTime)
{
}

void GameApplication::onRender(float fElapsedTime)
{
}
