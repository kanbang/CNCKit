/**
* @file    wordring/gui/container_window.cpp
*
* @brief   コンテナ・ウィンドウの実装ファイル
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

#include <wordring/gui/container_window.h>
#include <wordring/gui/canvas.h>

#ifdef _WIN32
#include <wordring/gui/detail/win32/win32_container_window.h>
#endif // _WIN32

#ifdef __linux__
// linux
#endif // __linux__

using namespace wordring::gui;

// container_window -----------------------------------------------------------

container_window::container_window()
	: window(new detail::native_container_window_impl)
{

}

container_window::container_window(detail::native_window* nw) : window(nw)
{

}

container_window::~container_window()
{

}
