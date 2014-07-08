/**
 * @file    wordring/gui/detail/win32/win32_window.h
 *
 * @brief   ウィンドウの環境固有ヘッダー・ファイル
 *
 * @details
 *        - このファイルは、win32専用です。
 *          ライブラリ利用者は、このヘッダー・ファイルを使用しません。\n
 *          pimplによって実装を隠ぺいします。
 *        - このファイルはwindow.cpp内に読み込まれます。 
 *        - メッセージ・マップはスレッドに対応していません。
 *          VC++がthread_localに対応した時点で対応を考えます。
 *          gcc4.8以降はthread_localに対応しているそうです。
 *          
 * @author  Kouichi Minami
 * @date    2014
 *
 * @par     ホーム
 *          https://github.com/wordring/
 * @par     ライセンス
 *          PDS
 */

#ifndef WORDRING_WIN32_WINDOW_H
#define WORDRING_WIN32_WINDOW_H

//#include <wordring/gui/window_service.h>
//#include <wordring/gui/detail/win32/win32_window_service.h>
#include <wordring/gui/detail/native_window.h>
#include <wordring/geometry/shape.h>

#include <Windows.h>

#include <thread>

#include <utility>
#include <map>
#include <cassert>

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

namespace wordring
{
namespace gui
{
namespace detail
{

// メッセージ・マップ ---------------------------------------------------------

class win32_message_map
{
	/// プログラム内唯一のマップ
	static std::map<HWND, native_window*> g_map;

public:
	/// 
	static void add_window(HWND hwnd, native_window* pwin)
	{
		g_map[hwnd] = pwin;
	}

	static native_window* find_window(HWND hwnd)
	{
		std::map<HWND, native_window*>::iterator it = g_map.find(hwnd);
		return (it == g_map.end()) ? nullptr : it->second;
	}

	static void remove_window(HWND hwnd)
	{
		size_t n = g_map.erase(hwnd);
		assert(n == 1);
	}
};

// ウィンドウ・クラス ---------------------------------------------------------

/**
 * @brief 基底ウィンドウ・クラス
 */
template <typename T>
struct win32_window_class
{
	HINSTANCE m_hinstance;
	ATOM m_atom;

	win32_window_class() : m_hinstance((HINSTANCE)NULL), m_atom((ATOM)NULL) { }

	virtual ~win32_window_class()
	{
		assert(m_atom != (ATOM)NULL);
		::UnregisterClass((LPCTSTR)(DWORD)m_atom, m_hinstance);
	}

	static WNDCLASSEX create_class()
	{
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = win32_window_class<T>::WindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(T*);
		wcex.hInstance = (HINSTANCE)::GetModuleHandle(NULL);
		wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
		wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = TEXT("win32_window_class<T>");
		wcex.hIconSm = NULL;

		return wcex;
	}

	static LRESULT CALLBACK WindowProc(
		HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		T* self = static_cast<T*>(win32_message_map::find_window(hWnd));

		if (uMsg == WM_CREATE || uMsg == WM_NCCREATE)
		{
			LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
			self = (T*)pcs->lpCreateParams;

			win32_message_map::add_window(
				hWnd, static_cast<native_window*>(self));

			self->m_hwnd = hWnd;
		}

		if (self)
		{
			return self->window_proc(hWnd, uMsg, wParam, lParam);
		}
		else
		{
			switch (uMsg)
			{
			case WM_GETMINMAXINFO:
				break;
			case WM_NCDESTROY:
				break;
			default:
#ifdef _DEBUG
				std::cout <<
					"ハンドルされないメッセージ: 0x";
				std::cout << std::hex << uMsg << std::endl;
#endif // _DEBUG
				break;
			}

		}

		return  ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
};

// 基本ウィンドウ -------------------------------------------------------------

/**
 * @brief 基本ウィンドウ
 * 
 * - native_window_implにtypedefされます。
 */
class win32_window_impl : public native_window
{
public:
	HWND m_hwnd;

public:
	win32_window_impl();

	virtual void create(window* parent);
	/// ウィンドウを最小化します
	virtual void close();

	virtual void destroy();

	virtual native_window* get_parent();
	virtual void set_parent(native_window* parent);

	virtual void set_size(size_int size);
	virtual size_int get_size() const;
	virtual void set_position(point_int point);
	virtual point_int get_position() const;

	LRESULT on_command(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void on_create();
	void on_click();

	LRESULT window_proc(
		HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	struct win32_window_impl_class
		: public win32_window_class<win32_window_impl>
	{
		win32_window_impl_class()
		{
			WNDCLASSEX wcex = create_class();
			m_atom = ::RegisterClassEx(&wcex);
			m_hinstance = wcex.hInstance;
		}

		~win32_window_impl_class(){}

		WNDCLASSEX create_class() const
		{
			WNDCLASSEX wcex =
				win32_window_class<win32_window_impl>::create_class();

			wcex.lpszClassName = TEXT("win32_window_impl_class");

			return wcex;
		}

		//static LRESULT CALLBACK WindowProc(
			//HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

	static win32_window_impl_class g_window_class;
};

class win32_control_window_impl : public win32_window_impl
{

};

class win32_container_window_impl : public win32_window_impl
{

};

class win32_button_window_impl : public win32_window_impl
{
public:
	virtual void create(window* parent);
};

/** window.cppで使用 */
typedef win32_window_impl native_window_impl;
typedef win32_button_window_impl native_button_window_impl;




} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_WINDOW_H
