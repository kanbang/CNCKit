/**
 * @file    wordring/gui/detail/win32/win32_window_service.cpp
 *
 * @brief   ウィンドウ・サービスのwin32環境固有実装ファイル
 *
 * @details
 *          一般的なGUIライブラリにおけるメッセージ・ポンプ相当です。\n
 *          pimplによって実装を隠蔽します。
 *          このファイルは、win32専用です。
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

#ifdef _WIN32

#include <wordring/debug.h>

//#include <wordring/wordring.h>
#include <wordring/gui/detail/win32/win32_window_service.h>

#include <Windows.h>

using namespace wordring::gui::detail;

__declspec(thread) win32_window_service_impl*
	win32_window_service_impl::tls_window_service = nullptr;

win32_window_service_impl::win32_window_service_impl()
{
	win32_window_service_impl::tls_window_service = this;
}

win32_window_service_impl::~win32_window_service_impl()
{

}

void win32_window_service_impl::run()
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

void win32_window_service_impl::quit()
{
	::PostQuitMessage(0); 
}




#endif // _WIN32
