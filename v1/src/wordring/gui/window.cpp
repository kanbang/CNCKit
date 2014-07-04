// wordring/gui/window.cpp

// Kouichi Minami 2014
// https://github.com/wordring/
// PDSとして提供

// ウィンドウ

#include <wordring/gui/window.h>

#ifdef _WIN32
#include <wordring/gui/detail/win32/win32_window.h>
#endif // _WIN32

#ifdef __linux__
// linux
#endif // __linux__

using namespace wordring::gui;

window::window() : m_native_ptr(new detail::native_window_impl)
{

}
