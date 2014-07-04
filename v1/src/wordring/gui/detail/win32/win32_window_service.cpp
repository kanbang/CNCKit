// wordring/gui/detail/win32/window_service.cpp

// Kouichi Minami 2014
// https://github.com/wordring/
// PDSとして提供

// ウインドウ・システムのメッセージ・ポンプを提供

#ifdef _WIN32

#include <wordring/gui/detail/win32/win32_window_service.h>
#include <Windows.h>

using namespace wordring::gui::detail;

void native_window_service_impl::run()
{

	MSG msg;
	BOOL result;
	while ((result = ::GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (result == -1) { break; }
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}






#endif // _WIN32
