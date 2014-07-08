/**
 * @file    wordring/gui/detail/win32/win32_window.cpp
 *
 * @brief   ウィンドウの環境固有実装ファイル
 *
 * @details
 *          win32専用のウィンドウ実装。\n
 *          pimplによって実装を隠ぺいします。
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

#include <wordring/gui/detail/win32/win32_window.h>
#include <wordring/gui/window.h>
#include <wordring/geometry/shape.h>

#include <Windows.h>

#include <algorithm>
#include <cassert>

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

using namespace wordring::gui;
using namespace wordring::gui::detail;

std::map<HWND, native_window*> win32_message_map::g_map;

// ウィンドウ・クラス ---------------------------------------------------------

/*
template <typename T>
LRESULT CALLBACK window_class<T>::WindowProc(
	HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	T* self = T::g_window_class.find_window(hWnd);

	//	(win32_window_impl*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (uMsg == WM_CREATE || uMsg == WM_NCCREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		self = (window_type*)pcs->lpCreateParams;

		T::g_window_class.add_window(hWnd, self);
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
		default:
#ifdef _DEBUG
			std::cout << "WM_CREATE 以前に送出されたハンドルされないメッセージ: 0x";
			std::cout << std::hex << uMsg << std::endl;
#endif // _DEBUG
			break;
		}

	}

	return  ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

*/
/*
template <typename T>
void window_map<T>::add(HWND hwnd, T* pT)
{
	m_handles.push_back(hwnd);
	m_objects.push_back(pT);
}

template <typename T>
T* window_map<T>::find(HWND hwnd)
{
	assert(m_handles.size() == m_objects.size());

	std::vector<HWND>::iterator it =
		std::find(m_handles.begin(), m_handles.end(), hwnd);
	if (it == m_handles.end()) { return m_current;  }

	return m_objects.at(it - m_handles.begin());
}

*/
// 基本ウィンドウ -------------------------------------------------------------

win32_window_impl::win32_window_impl()
{
	//win32_window_impl::g_window_class.m_map.set_current(this);


	//win32_window_impl::g_window_class.m_map.remove_current();

	//win32_window_impl::g_window_class.m_map.add(m_hwnd, this);

	


}

void win32_window_impl::create(window* parent)
{
	HWND hparent = NULL;
	if (parent)
	{
		hparent = dynamic_cast<win32_window_impl*>(parent->get_native())->m_hwnd;
	}

	m_hwnd = ::CreateWindow(
		(LPCTSTR)(DWORD)win32_window_impl::g_window_class.m_atom,
		L"Test", WS_OVERLAPPEDWINDOW,
		100,
		100,
		200,
		200,
		hparent,
		NULL,
		win32_window_impl::g_window_class.m_hinstance,
		this);
	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);
}

void win32_window_impl::close()
{
	BOOL result = ::CloseWindow(m_hwnd);
	assert(result != 0);
}

void win32_window_impl::destroy()
{
	BOOL result = ::DestroyWindow(m_hwnd);
	assert(result != 0);
}

native_window* win32_window_impl::get_parent()
{
	HWND hwnd = ::GetAncestor(m_hwnd, GA_PARENT);
	native_window* result = win32_message_map::find_window(hwnd);
	assert(result != nullptr);

	return result;
}

void win32_window_impl::set_parent(native_window* parent)
{
	win32_window_impl* p = dynamic_cast<win32_window_impl*>(parent);
	::SetParent(m_hwnd, p->m_hwnd);
}

void win32_window_impl::set_size(size_int size)
{
	UINT flags = SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	BOOL result = ::SetWindowPos(m_hwnd, 0, 0, 0, size.h, size.w, flags);
	assert(result != 0);
}

size_int win32_window_impl::get_size() const
{
	RECT rect;
	BOOL result = ::GetClientRect(m_hwnd, &rect);
	assert(result != 0);
	return size_int(rect.right, rect.bottom);
}

void win32_window_impl::set_position(point_int point)
{
	UINT flags = SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	BOOL result = ::SetWindowPos(m_hwnd, 0, point.x, point.y, 0, 0, flags);
	assert(result != 0);
}

point_int win32_window_impl::get_position() const
{
	RECT rect;
	BOOL result = ::GetWindowRect(m_hwnd, &rect);
	assert(result != 0);
	//return size_int(rect.right, rect.bottom);
	return point_int(0, 0);
}

LRESULT win32_window_impl::window_proc(
	HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		return on_command(hWnd, uMsg, wParam, lParam);
	case WM_CREATE:
		on_create();
		break;
	case WM_DESTROY:
		win32_message_map::remove_window(m_hwnd);
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);

}

LRESULT win32_window_impl::on_command(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = (HWND)lParam;
	assert(hwnd);
	window* win = win32_message_map::find_window(hwnd)->get_window();
	win->on_click();

	return 0;
}

void win32_window_impl::on_create()
{
	std::function<void()> fn = get_window()->on_create;
	if (fn) { fn(); }
}

void win32_window_impl::on_click()
{
	std::function<void()> fn = get_window()->on_click;
	if (fn) { fn(); }
}

win32_window_impl::win32_window_impl_class win32_window_impl::g_window_class;

void win32_button_window_impl::create(window* parent)
{
	HWND hparent = NULL;
	if (parent)
	{
		hparent = dynamic_cast<win32_window_impl*>(parent->get_native())->m_hwnd;
	}

	m_hwnd = ::CreateWindow(
		TEXT("BUTTON"),
		L"Button", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		60,
		60,
		60,
		60,
		hparent,
		NULL,
		win32_window_impl::g_window_class.m_hinstance,
		NULL);

	win32_message_map::add_window(m_hwnd, this);
	//::ShowWindow(m_hwnd, SW_SHOW);
	//::UpdateWindow(m_hwnd);

}





#endif // _WIN32
