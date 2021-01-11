/*************************************************************

gibbie-03
Copyright (c) 2020-2020, Gianluca Belardelli

File:    gbe_basewindow.h
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
#include "gbe_types.h"



class GBEBaseWindow
{
public:
	virtual ~GBEBaseWindow() {}
	virtual void setActive(bool active) = 0;
	virtual void setPosition(int xPos, int yPos) = 0;
	virtual void setSize(int width, int height) = 0;
	virtual void setTitle(std::string title) = 0;

	virtual void show(bool update) = 0;
	virtual void close() = 0;
	virtual bool pollEvents() = 0;
	virtual GBEWINDOWHANDLE getNativeHandle() = 0;

	static GBEBaseWindow* CreateGBEWindow(std::string &title, int xPos, int yPos, int width, int height, unsigned int flags);
	static GBEBaseWindow* CreateGBEWindow(std::string& title, GBEWindowStyle flags = GBEWindowDefaultNoResize);
};
