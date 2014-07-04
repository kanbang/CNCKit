// wordring/gui/window_service.cpp

// Kouichi Minami 2014
// https://github.com/wordring/
// PDSとして提供

// ウインドウ・システムのメッセージ・ポンプを提供

#include <wordring/gui/window_service.h>

#ifdef _WIN32
#include <wordring/gui/detail/win32/win32_window_service.h>
#endif // _WIN32

#ifdef __linux__
// linux
#endif // __linux__

wordring::gui::window_service::window_service()
	: m_native_ptr(new wordring::gui::detail::native_window_service_impl)
{

}

void wordring::gui::window_service::run()
{
	m_native_ptr->run();
}
