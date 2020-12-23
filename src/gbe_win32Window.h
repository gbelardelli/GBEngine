/*************************************************************

gibbie-02
Copyright (c) 2020-2020, Gianluca Belardelli

File:    gbe_win32Window.h
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

#include <Windows.h>
#include <string>

class GBEWin32Window : public GBEBaseWindow
{
	GBEWin32Window(const GBEWin32Window&) = delete;
	GBEWin32Window& operator=(const GBEWin32Window&) = delete;

	friend GBEWin32Window* __CreateWindowInternal(std::string &title, int xPos, int yPos, int width, int height, unsigned int flags);

private:
	HWND	mWnd;
	HDC		mDC = nullptr;
	WNDPROC mWndProc;

	std::string mTitle;

protected:
	GBEWin32Window(HWND hWnd, WNDPROC wndProc = nullptr);

public:
	GBEWin32Window() = delete;
	
	~GBEWin32Window();
	void setActive(bool active) override final;
	void setPosition(int xPos, int yPos) override final;
	void setSize(int width, int height) override final;
	void setTitle(std::string title) override final;

	void show(bool update) override final;
	void close() override final;
	bool pollEvents() override final;
	void* getNativeHandle() override final;

};
