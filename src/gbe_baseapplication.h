/*************************************************************

gibbie-02
Copyright (c) 2020-2020, Gianluca Belardelli

File:    gbe_baseapplication.h
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
#pragma once

#include <string>

class GBEBaseApplication
{
protected:
	std::string	mAppName;
	uint32_t mAppFlags;

private:
	bool init();
	void mainLoop();
	void cleanup();

	virtual void onInitialize() = 0;
	virtual void onFinalize(void) = 0;
	virtual void onUpdate(float fElapsedTime) = 0;
	virtual void onRender(float fElapsedTime) = 0;
public:
	GBEBaseApplication() = delete;
	GBEBaseApplication(std::string& appName, uint32_t appFlags = 0);
	
	virtual ~GBEBaseApplication();
	
	void run();
};

