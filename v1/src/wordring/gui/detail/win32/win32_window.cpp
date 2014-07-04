// wordring/gui/detail/win32/window.cpp

// Kouichi Minami 2014
// https://github.com/wordring/
// PDSとして提供

// ウィンドウ

#ifdef _WIN32

#include <wordring/gui/detail/win32/win32_window.h>

#include <Windows.h>

using namespace wordring::gui::detail;

LRESULT CALLBACK win32_window_traits::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

win32_window_traits::win32_window_traits()
{
	m_wcex.cbClsExtra = 0;
	m_wcex.cbWndExtra = 0;
	m_wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	m_wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	m_wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	m_wcex.hInstance = (HINSTANCE)::GetModuleHandle(NULL);
	m_wcex.lpfnWndProc = WindowProc;
	m_wcex.lpszClassName = L"native_window_traits_impl";
	m_wcex.lpszMenuName = NULL;
	m_wcex.style = CS_HREDRAW | CS_VREDRAW;

	if (!::RegisterClassEx(&m_wcex)) { return; }
}

native_window_impl::native_window_impl()
{

}






#endif // _WIN32
