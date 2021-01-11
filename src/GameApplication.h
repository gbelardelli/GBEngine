#pragma once

#include "gbe_baseapplication.h"

class GameApplication : public GBEBaseApplication
{
protected:
	bool onInitialize() override final;
	void onFinalize(void) override final;
	void onUpdate(float fElapsedTime) override final;
	void onRender(float fElapsedTime) override final;

public:
	GameApplication();
	~GameApplication();


};