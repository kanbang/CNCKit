/**
* @file    wordring/gui/native/win32/win32_canvas.h
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

#ifndef WORDRING_WIN32_CANVAS_H
#define WORDRING_WIN32_CANVAS_H

#include <wordring/debug.h>

#include <wordring/gui/detail/native_canvas.h>

#include <Windows.h>

namespace wordring
{
namespace gui
{
namespace detail
{

class native_canvas_impl : public native_canvas
{
protected:
	HDC m_hdc;

public:
	native_canvas_impl();
	native_canvas_impl(HDC hdc);
	virtual ~native_canvas_impl();

	HDC get_dc();

	virtual void draw_string(std::string str, point_int pt);
	virtual void draw_string(std::wstring str, point_int pt);
};

class native_memory_canvas_impl : public native_canvas_impl
{
protected:
	HBITMAP m_bitmap;
	BYTE* m_bits;

public:
	native_memory_canvas_impl();
	virtual ~native_memory_canvas_impl();

};


} // detail
} // namespace gui
} // namespace wordring

#endif // WORDRING_WIN32_CANVAS_H
