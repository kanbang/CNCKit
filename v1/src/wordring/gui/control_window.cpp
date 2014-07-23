/**
* @file    wordring/gui/control_window.cpp
*
* @brief   ウィンドウの実装ファイル
*
* @details
*          pimplの環境固有ヘッダをこのファイルにのみインクルードしてください。
*
*
*
* @author  Kouichi Minami
* @date    2014
*
* @par     ホーム
*          https://github.com/wordring/
* @par     ライセンス
*          PDS
*/

#include <wordring/debug.h>

#include <wordring/gui/control_window.h>

#ifdef _WIN32
#include <wordring/gui/detail/win32/win32_control_window.h>
#endif // _WIN32

#ifdef __linux__
// linux
#endif // __linux__

using namespace wordring::gui;

// control_window -------------------------------------------------------------

control_window::control_window()
	: window(new detail::native_control_window_impl)
{
}

control_window::control_window(detail::native_window* nw) : window(nw)
{
}

control_window::~control_window()
{

}
