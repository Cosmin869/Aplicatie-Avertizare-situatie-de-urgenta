#include "Window.h"


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_LBUTTONDOWN:
	{
		wchar_t szFileName[MAX_PATH];
		HINSTANCE hInstance = GetModuleHandle(NULL);

		GetModuleFileName(hInstance, szFileName, MAX_PATH);
		MessageBox(hWnd, szFileName, L"This program is:", MB_OK | MB_ICONINFORMATION);
	}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Window::Window():hInstance(GetModuleHandle(nullptr))
{
	const wchar_t* CLASS_NAME = L"Window Class";

	WNDCLASS wndClass = {};
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = this->hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wndClass.lpfnWndProc = WindowProc;

	RegisterClass(&wndClass);

	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	int width = 640;
	int height = 480;

	RECT rect;
	rect.left = 250;
	rect.top = 250;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	AdjustWindowRect(&rect, style, false);

	this->hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Title",
		style,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		NULL,
		this->hInstance,
		NULL
	);

	ShowWindow(this->hWnd, SW_SHOW);
}

Window::~Window() {
	const wchar_t* CLASS_NAME = L"Window Class";

	UnregisterClass(CLASS_NAME, this->hInstance);
	 
}

bool Window::ProcessMessage() {

	MSG msg = {};
	
	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return false;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}
