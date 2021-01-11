/*************************************************************

gibbie-03
Copyright (c) 2020-2020, Gianluca Belardelli

File:    gbe_win32Window.cpp
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
#include "gbe_basewindow.h"
#include "gbe_win32Window.h"

GBEWin32Window::GBEWin32Window(HWND hWnd, WNDPROC wndProc) : mWnd(hWnd)
{
	mDC = GetDC(hWnd);

	if (wndProc == nullptr)
		mWndProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
	else
		mWndProc = wndProc;

	char szTitle[128];
	GetWindowText(hWnd, szTitle, 128);

	mTitle = szTitle;
}

GBEWin32Window::~GBEWin32Window()
{
	if(mDC)
		ReleaseDC(mWnd, mDC);

	DestroyWindow(mWnd);
}

void GBEWin32Window::setActive(bool active)
{
}

void GBEWin32Window::setPosition(int xPos, int yPos)
{
	SetWindowPos(mWnd, mWnd, xPos, yPos, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void GBEWin32Window::setSize(int width, int height)
{
	SetWindowPos(mWnd, mWnd, -1, -1, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void GBEWin32Window::setTitle(std::string title)
{
	SetWindowText(mWnd, title.c_str());
}

void GBEWin32Window::show(bool update)
{
	if (update)
		UpdateWindow(mWnd);

	ShowWindow(mWnd, SW_SHOW);
}

void GBEWin32Window::close()
{
	CloseWindow(mWnd);
}

GBEWINDOWHANDLE GBEWin32Window::getNativeHandle()
{
	return mWnd;
}

bool GBEWin32Window::pollEvents()
{
	MSG msg;
	bool bQuit = false;

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			bQuit = true;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (GetAsyncKeyState(VK_ESCAPE))
		bQuit = true;

	return bQuit;
}


#define GBE_WND_CLASSNAME	"GIBBIEngineWndClass"

LRESULT CALLBACK wndProc(HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	switch(uMsg)
	{
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool _registerWindowClass(void)
{
	WNDCLASSEX wc;

	if( GetClassInfoEx(GetModuleHandle(nullptr), GBE_WND_CLASSNAME, &wc) )
		return true;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)wndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.hIcon = nullptr;
	wc.hIconSm = nullptr;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GBE_WND_CLASSNAME;

	if (!RegisterClassEx(&wc))
		return false;

	return true;
}

GBEWin32Window* __CreateWindowInternal(std::string &title, int xPos, int yPos, int width, int height, unsigned int flags)
{
	if (!_registerWindowClass())
		return nullptr;

	DWORD dwStyle = WS_VISIBLE;
	DWORD dwStyleEx = WS_EX_APPWINDOW;

	if (flags == 0)
	{
		dwStyle |= WS_POPUP;
	}
	else
	{
		if (flags & GBEWindowStyle::GBEWindowHaveTitlebar)
			dwStyle |= WS_CAPTION | WS_MINIMIZEBOX;

		if (flags & GBEWindowStyle::GBEWindowCanResize)
			dwStyle |= WS_THICKFRAME | WS_MAXIMIZEBOX;

		if (flags & GBEWindowStyle::GBEWindowCanClose)
			dwStyle |= WS_SYSMENU;

		if (!(flags & GBEWindowStyle::GBEWindowFullscreen))
		{
			RECT rcRect = { 0, 0, width, height };
			AdjustWindowRect(&rcRect, dwStyle, false);
			width = rcRect.right - rcRect.left;
			height = rcRect.bottom - rcRect.top;
			dwStyleEx |= WS_EX_WINDOWEDGE;
		}
	}

	dwStyle |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	HWND hWnd = CreateWindowEx(dwStyleEx, GBE_WND_CLASSNAME, title.c_str(), 
		dwStyle, xPos, yPos, width, height, 
		nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

	if (hWnd)
	{
		GBEWin32Window* gbeWnd = new GBEWin32Window(hWnd);
		if (gbeWnd == nullptr)
			return nullptr;

		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)gbeWnd);

		if(flags & GBEWindowStyle::GBEWindowCentered)
		{
			// Center the window
			RECT rcRect;

			GetWindowRect(hWnd, &rcRect);

			const int screenX = (GetSystemMetrics(SM_CXSCREEN) - rcRect.right) / 2;
			const int screenY = (GetSystemMetrics(SM_CYSCREEN) - rcRect.bottom) / 2;

			gbeWnd->setPosition(screenX, screenY);
		}

		if (flags & GBEWindowStyle::GBEWindowFullscreen)
			ShowCursor(FALSE);

		/*AEPlatformConfig* lpPlatform = AEngine::GetDefaultVideoConfig()->GetSpecificPlatformInfo();
		lpPlatform->hWnd = hWnd;
		lpPlatform->lpWindow = lpWnd;

		AEngine::GetDefaultVideoConfig()->SetSpecificPlatformInfo(lpPlatform);*/

		return gbeWnd;
	}

	return nullptr;
}

GBEBaseWindow* GBEBaseWindow::CreateGBEWindow(std::string &title, int xPos, int yPos, int width, int height, unsigned int flags)
{
	return __CreateWindowInternal(title, xPos, yPos, width, height, flags);
}

GBEBaseWindow* GBEBaseWindow::CreateGBEWindow(std::string& title, GBEWindowStyle flags)
{
	return __CreateWindowInternal(title, 0, 0, 800, 600, flags);
}
