/**
 * @file    wordring/gui/detail/win32/win32_window_class.h
 *
 * @brief   
 *
 * @details
 *          
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

#ifndef WORDRING_WIN32_WINDOW_CLASS_H
#define WORDRING_WIN32_WINDOW_CLASS_H

#include <wordring/debug.h>

#include <wordring/gui/detail/native_window.h>
#include <wordring/gui/detail/win32/win32_window_service.h>

#include <Windows.h>
#include <windowsx.h>

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <cassert>

namespace wordring
{
namespace gui
{
namespace detail
{

// ウィンドウ・クラス ---------------------------------------------------------

/**
 * @brief   基底ウィンドウ・クラス
 * @details 
 *          win32のウィンドウ・クラスを作る目的の基底クラスです。
 *
 * @param   T1 継承されたウィンドウ・クラス
 * @param   T2 ウィンドウ実装
 */
template <typename T1, typename T2>
struct win32_window_class
{
	typedef typename T1 class_type;
	typedef typename T2 window_type;

	HINSTANCE m_hinstance;
	ATOM m_atom;

	/// win32のウィンドウ・クラスを登録します
	win32_window_class() : m_hinstance((HINSTANCE)NULL), m_atom((ATOM)NULL)
	{
		WNDCLASSEX wcex = T1::create();
		m_atom = ::RegisterClassEx(&wcex);
		m_hinstance = wcex.hInstance;
	}

	/// win32のウィンドウ・クラスを登録解除します
	virtual ~win32_window_class()
	{
		assert(m_atom != (ATOM)NULL);
		::UnregisterClass((LPCTSTR)(DWORD)m_atom, m_hinstance);
	}

	/// ウィンドウ・プロシージャの雛型です
	static LRESULT CALLBACK WindowProc(
		HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		native_window* nw = nullptr;
		window_type* iw = nullptr;

		if (uMsg == WM_NCCREATE)
		{
			LPCREATESTRUCT cs = (LPCREATESTRUCT)lParam;
			nw = static_cast<native_window*>(cs->lpCreateParams);
			assert(nw);
			iw = static_cast<window_type*>(nw);
			iw->m_hwnd = hwnd;
			win32_window_service_impl::assign(hwnd, nw);
		}

		LRESULT result = 0;
		bool handled = false;

		iw = static_cast<window_type*>(
			win32_window_service_impl::find(hwnd));
		if(iw) // WM_NCCREATE以前は登録されていない
		{
			result = iw->WindowProc(hwnd, uMsg, wParam, lParam);
			handled = iw->get_message_handled();
		}
		else
		{
			// TODO: ウィンドウに配送できないメッセージの処理
		}

		if (uMsg == WM_NCDESTROY)
		{
			win32_window_service_impl::remove(hwnd);
			assert(iw); // 登録されていないウィンドウを消すことはできない
			iw->m_hwnd = nullptr;
		}

		if (!handled) { result = ::DefWindowProc(hwnd, uMsg, wParam, lParam); }

		return result;
	}
};

} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_WINDOW_CLASS_H
