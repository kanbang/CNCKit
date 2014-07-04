/*
@file    wordring/gui/detail/win32/win32_window.h
@brief   環境固有のウィンドウ実装

@author  Kouichi Minami
@date    2014/07/03
*/

// https://github.com/wordring/
// PDSとして提供

#ifndef WORDRING_WIN32_WINDOW_H
#define WORDRING_WIN32_WINDOW_H

#include <wordring/gui/detail/native_window.h>
#include <Windows.h>

namespace wordring
{
namespace gui
{
namespace detail
{

// 
class win32_window_traits
{
private:
	WNDCLASSEX m_wcex;

public:
	win32_window_traits();

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};

class native_window_impl : public native_window
{
private:
public:
	native_window_impl();
};







} // namespace detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_WINDOW_H
