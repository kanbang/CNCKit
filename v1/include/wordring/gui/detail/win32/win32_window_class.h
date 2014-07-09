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

#include <wordring/gui/detail/native_window.h>

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

	/// win32のウィンドウ・クラス解除します
	virtual ~win32_window_class()
	{
		assert(m_atom != (ATOM)NULL);
		::UnregisterClass((LPCTSTR)(DWORD)m_atom, m_hinstance);
	}

	/// ウィンドウ・プロシージャの雛型です
	static LRESULT CALLBACK WindowProc(
		HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		bool flg = false;

		window* w = win32_window_service_impl::find(hwnd);
		if(w)
		{
			ImplT* pT = static_cast<ImplT*>(w->get_native_window());
			assert(pT);
			return pT->WindowProc(&flg, hwnd, uMsg, wParam, lParam);
		}

	}



};

} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_WINDOW_CLASS_H
