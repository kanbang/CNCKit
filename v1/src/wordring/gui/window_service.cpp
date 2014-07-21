/**
 * @file    wordring/gui/window_service.cpp
 *
 * @brief   ウィンドウ・サービスの実装ファイル
 *
 * @details
 *          一般的なGUIライブラリにおけるメッセージ・ポンプ相当です。
 *          
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

#include <wordring/gui/window_service.h>

#ifdef _WIN32
#include <wordring/gui/detail/win32/win32_window_service.h>
#endif // _WIN32

#ifdef __linux__
// linux
#endif // __linux__

using namespace wordring::gui;

window_service::window_service()
	: m_native(new wordring::gui::detail::native_window_service_impl)
{

}


detail::native_window_service* window_service::get_native_window_service()
{
	return m_native.get();
}

void window_service::run()
{
	m_native->run();
}

void window_service::quit()
{
	m_native->quit();
}

