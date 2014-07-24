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
* @brief 基底ウィンドウ・クラス
* @details
*    win32のウィンドウ・クラスを作る目的の基底クラスです。
*    C++のクラスとwin32のウィンドウ・クラスを結びつけます。
*    仮想関数を使わずにメッセージを配送する能力があります。
*/
template <typename ClsT, typename ImplT>
struct win32_window_class
{
	HINSTANCE m_hinstance;
	ATOM m_atom;

	/// win32のウィンドウ・クラスを登録します
	win32_window_class() : m_hinstance((HINSTANCE)NULL), m_atom((ATOM)NULL)
	{
		WNDCLASSEX wcex = ClsT::create();
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
		if (uMsg == WM_NCCREATE)
		{
			LPCREATESTRUCT cs = (LPCREATESTRUCT)lParam;
			native_window* nw =
				static_cast<native_window*>(cs->lpCreateParams);
			assert(nw);
			ImplT* iw = static_cast<ImplT*>(nw);
			iw->m_hwnd = hwnd;
			win32_window_service_impl::assign(hwnd, nw);
		}

		LRESULT result = 0;
		bool handled = false;

		ImplT* iw = static_cast<ImplT*>(win32_window_service_impl::find(hwnd));
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
