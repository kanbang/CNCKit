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
#define WINVER 0x0500 
#include <wordring/debug.h>

#include <wordring/exception.h>

#include <wordring/gui/window_service.h>
#include <wordring/gui/detail/win32/win32_window_service.h>

#include <Windows.h>

#include <cassert>
#include <iostream>

using namespace wordring::gui::detail;

__declspec(thread) win32_window_service_impl*
	win32_window_service_impl::tls_window_service = nullptr;

win32_window_service_impl::win32_window_service_impl()
{
	win32_window_service_impl::tls_window_service = this;
	m_events[0] = ::CreateEvent(NULL, TRUE, FALSE, NULL);
}

win32_window_service_impl::~win32_window_service_impl()
{
	::CloseHandle(m_events[0]);
}

/*
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
*/

void win32_window_service_impl::run()
{
	MSG msg;
	BOOL result;

	for (;;)
	{
		result = ::MsgWaitForMultipleObjects(
			1, m_events, FALSE, INFINITE, QS_ALLINPUT);

		if (result == WAIT_OBJECT_0)
		{
			get_public()->do_tick_message();
			::ResetEvent(m_events[0]);
		}

		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) { return; }

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		while (::PeekMessage(&msg, (HWND)-1, 0, 0, PM_REMOVE))
		{
		}

	}
}

void win32_window_service_impl::quit()
{
	::PostQuitMessage(0); 
}

void win32_window_service_impl::post_tick_message()
{
	::SetEvent(m_events[0]);
	/*
	if (result == 0)
	{
		std::string err =
			wordring::get_exception_category().message(::GetLastError());
	}
	wordring::check_assertion(result != 0);
	assert(result != 0);
	*/
}

void win32_window_service_impl::assign(HWND hwnd, native_window* pwin)
{
	win32_window_service_impl::tls_window_service->m_map[hwnd] = pwin;
}


#endif // _WIN32
