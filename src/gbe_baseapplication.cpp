/*************************************************************

gibbie-03
Copyright (c) 2020-2020, Gianluca Belardelli

File:    gbe_baseapplication.cpp
Author:  Gianluca Belardelli
Date:    22/12/2020

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

**************************************************************/
#include "gbe_baseapplication.h"
#include "gbe_basewindow.h"

GBEBaseApplication::GBEBaseApplication(std::string appName, uint32_t appFlags) :
	mAppName(appName), mAppFlags(appFlags), mMainWindow(nullptr)
{
}

GBEBaseApplication::~GBEBaseApplication()
{
	if (mMainWindow)
		delete mMainWindow;
}

void GBEBaseApplication::run()
{
	if (!init())
		return;

	mainLoop();

	cleanup();
}

bool GBEBaseApplication::init()
{
	if (mMainWindow)
		return false;

	mMainWindow = GBEBaseWindow::CreateGBEWindow(mAppName);
	
	if (!onInitialize())
		return false;

	mMainWindow->show(true);

	return true;
}

void GBEBaseApplication::mainLoop()
{
	for(;;)
	{
		if (mMainWindow->pollEvents())
			break;
	}
}

void GBEBaseApplication::cleanup()
{
	onFinalize();

	mMainWindow->close();
}
